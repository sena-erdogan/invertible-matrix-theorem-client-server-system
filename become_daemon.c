#include <sys/stat.h>
#include <fcntl.h>
#include "become_daemon.h"

int becomeDaemon(int flags)
{
	int maxfd, fd;
	switch (fork()) { /* Become background process */
		case -1: return -1;
		case 0: break; /* Child falls through... */
		default: _exit(0); /* while parent terminates */
	}
	if (setsid() == -1) /* Become leader of new session */
		return -1;
	switch (fork()) { /* Ensure we are not session leader */
		case -1: return -1;
		case 0: break;
		default: _exit(0);
	}
	if (!(flags & BD_NO_UMASK0))
		umask(0); /* Clear file mode creation mask */
	if (!(flags & BD_NO_CHDIR))
		chdir("/"); /* Change to root directory */
	/*if (!(flags & BD_NO_CLOSE_FILES)) {
		maxfd = sysconf(_SC_OPEN_MAX);
		if (maxfd == -1) 
			maxfd = BD_MAX_CLOSE;
		for (fd = 0; fd < maxfd; fd++)
			close(fd);
	}
	if (!(flags & BD_NO_REOPEN_STD_FDS)) {
		close(STDIN_FILENO);
		fd = open("/dev/null", O_RDWR);
		if (fd != STDIN_FILENO) 
			return -1;
		if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
			return -1;
		if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
			return -1;
	}*/
	return 0;
}
