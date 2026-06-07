#include <sys/wait.h>
#include <sys/reboot.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void reap(int sig) {
	(void)sig;
	while (waitpid(-1, NULL, WNOHANG) > 0) {}
}

int main() {
	struct sigaction sa = {0};
	sa.sa_handler = reap;
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	sigaction(SIGCHLD, &sa, NULL);

	int status;
	pid_t pid;
	pid = fork();
	if (pid < 0) {
		return 1;
	} if (getpid() == pid) {
		return execl("/kizinit/init", "init", NULL);
	}

	waitpid(pid, &status, 0);
	if (WIFEXITED(status)) {
		int returned = WEXITSTATUS(status);
		if (returned == 0) {
			sync();
			reboot(RB_POWER_OFF, NULL);
		}
		if (returned == 1) {
			sync();
			reboot(RB_AUTOBOOT, NULL);
		}
		if (returned == -1) {
			sync();
			reboot(RB_HALT_SYSTEM, NULL);
		}
	}
}
