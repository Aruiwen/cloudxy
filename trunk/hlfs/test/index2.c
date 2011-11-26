/*
 *  test5.c
 *
 *  Harry Wei <harryxiyou@gmail.com> (C) 2011
 */
#include <string.h>
#include <glib.h>
#include "_hlfs_ctrl.h"

/*
 * Close the lhdfs execution and then restart. Invoke the hlfs_read
 * read the position of 512k datas.
 * Check: read the position of 512k with the length of 64b, check
 *        if the datas are right.
 */

int main(void)
{	
	int i;
	g_print("test 5 >>> \n");
	char content2[64];
	memset(content2,'a',64);
	const char *uri = "local:///tmp/testenv";
	const char *fs_name = "testfs";
	struct _hlfs_ctrl *ctrl = init_hlfs(uri, fs_name);
	
	g_assert(ctrl != NULL);
	g_print("test hlfs open >>> \n");
	int ret = 0;
	ret = hlfs_open(ctrl, 1);
	g_assert(ret == 0);
	ret = hlfs_write(ctrl,content2, 64lu, 64llu * 8llu * 1024llu);//begin from the first of second inode
	g_assert(ret>0);
	g_print("test hlfs read >>> \n");
	ret = hlfs_read(ctrl, content2, 64lu, 64llu * 8llu * 1024llu);
	g_assert(ret > 0);
	for (i = 0; i < 64; i++) {
		g_message("%c",content2[i]);;
	}
	return 0;
}
