#!/bin/sh

tar zcvf freyja-trunk-snapshot-`date +%Y%m%d`.tar.gz --exclude=.svn /usr/local/bin/freyja /usr/local/lib/libfreyja.so.0.9.3 /usr/local/lib/libfreyja.so /usr/local/lib/libhel0.so.0.0.1 /usr/local/lib/libhel0.so /usr/local/lib/libmgtk.so.0.1.0 /usr/local/lib/libmgtk.so /usr/local/lib/freyja/modules/image/*.so  /usr/local/lib/freyja/modules/model/*.so /usr/share/freyja/ 
