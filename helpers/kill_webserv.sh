pid=$(lsof -i -P -n | grep LISTEN | grep webserv | awk '{print $2}')
kill -9 $pid
