/*
 *  src/snapshot/hlfs_take_snapshot.c
 *
 *  Harry Wei <harryxiyou@gmail.com> (C) 2011
 *  updated by Kelvin <kelvin.xupt@gmail.com>
 */
#include <stdio.h>
#include <stdint.h>
#include <glib.h>
#include <string.h>
#include "hlfs_ctrl.h"
#include "snapshot.h"
#include "storage_helper.h"
#include "hlfs_log.h"
#include "misc.h"
#include "comm_define.h"

int hlfs_take_snapshot(struct hlfs_ctrl *ctrl, const char *ssname) 
{
    if(ctrl == NULL || ssname ==NULL){
		HLOG_ERROR("parameter error!");
        return -1;
    }
    int ret = 0;

    g_mutex_lock(ctrl->hlfs_access_mutex);
    if(ctrl->rw_inode_flag == 0){
		HLOG_ERROR("error, snapshot can not take when readonly");
        g_mutex_unlock (ctrl->hlfs_access_mutex);
        return -1;
    }
    g_mutex_unlock (ctrl->hlfs_access_mutex);
	/* record the up snapshot name in ctrl */
	if (NULL == ctrl->alive_ss_name) {
		HLOG_DEBUG("this is first time take snapshot");
	}
    struct snapshot *_ss = NULL;
	if (0!=(ret=load_snapshot_by_name(ctrl->storage,SNAPSHOT_FILE,&_ss,ssname))){
		HLOG_DEBUG("snapshot %s is not exist, right???", ssname);
		return -1;
	}
	struct snapshot ss;
	ss.timestamp = get_current_time();
	if ((strlen(ssname) + 1) > HLFS_FILE_NAME_MAX) {
		HLOG_ERROR("error, snapshot name beyond max length!");
		return -1;
	}
	g_strlcpy(ss.sname, ssname, strlen(ssname) + 1);
    g_mutex_lock(ctrl->hlfs_access_mutex);
	g_strlcpy(ss.up_sname,ctrl->alive_ss_name,strlen(ctrl->alive_ss_name) + 1);
	ss.inode_addr = ctrl->imap_entry.inode_addr;
	memset(ctrl->alive_ss_name, 0, (strlen(ctrl->alive_ss_name) + 1));
	sprintf(ctrl->alive_ss_name, "%s", ss.sname);
    g_mutex_unlock (ctrl->hlfs_access_mutex);

    ret = dump_alive_snapshot(ctrl->storage,ALIVE_SNAPSHOT_FILE,&ss);
    if(ret!=0){
      HLOG_ERROR("dump snapshot alive error!");
    }
	ret = dump_snapshot(ctrl->storage,SNAPSHOT_FILE,&ss);
    if(ret!=0){
      HLOG_ERROR("dump snapshot error!");
    }
	return ret;
}
