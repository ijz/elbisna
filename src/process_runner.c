#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <sys/wait.h>
#include <unistd.h>

#define FAIL_EXIT(msg)                                  \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define FAIL__EXIT(msg)                                 \
    do { perror(msg); _exit(EXIT_FAILURE); } while (0)

#define FAIL_RETURN(msg, r)                     \
    do { perror(msg); return r; } while (0)


static siginfo_t siginfo;
static pid_t child_pids[16] = {0};
static size_t pid_ptr = 0;
static int sfd;

int fork_and_run(const char *file, char **argv) {
    pid_t chpid = fork();
    if (0 > chpid)
        FAIL_RETURN("fork", -1);
    if (0 == chpid) {
        // child process
        // pretend it takes a while
        sleep(5);
        if (0 > execvp(file, argv))
            FAIL__EXIT("execvp");
    } else {
        // parent process
        child_pids[pid_ptr++] = chpid;
    }
    return 0;
}

int reap_children() {
    ssize_t s;
    struct signalfd_siginfo sfdsi;
    size_t sfdsi_size = sizeof(struct signalfd_siginfo);
    for (;;) {
        s = read(sfd, &sfdsi, sfdsi_size);
        if (s != sfdsi_size) {
            perror("reap_children:read");
            continue;
        }
        switch (sfdsi.ssi_signo) {
        case SIGINT:
            printf("got SIGINT\n");
            exit(EXIT_SUCCESS);
        case SIGCHLD:
            printf("got SIGCHLD\n");
            for (;;) {
                memset(&siginfo, 0, sizeof(siginfo));
                if (0 > waitid(P_ALL, 0, &siginfo, WEXITED | WNOHANG)) {
                    perror("waitid");
                    break;
                }
                // no more events for this SIGCHLD
                if (siginfo.si_pid == 0)
                    break;
                printf("child_pid: %d\n", siginfo.si_pid);
                printf("all pids: ");
                pid_t *p;
                for (p = child_pids; p != child_pids + pid_ptr; ++p) {
                    printf("%d, ", *p);
                }
                printf("\n");
            }
        }
    }
}

int setup_signal_fd() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGCHLD);

    if (0 > sigprocmask(SIG_BLOCK, &mask, NULL))
        FAIL_RETURN("sigprocmask", -1);
    if (0 > (sfd = signalfd(-1, &mask, SFD_CLOEXEC)))
        FAIL_RETURN("signalfd", -1);
    return 0;
}

int main(int argc, char **argv) {
    if (0 > setup_signal_fd())
        FAIL_EXIT("setup_signal_fd");
    int i;
    char str[10] = {0};
    char *args[3] = {0};
    for (i = 0; i < 10; ++i) {
        if (0 > sprintf(str, "test%d", i)) {
            perror("sprintf");
            continue;
        }
        args[0] = "touch";
        args[1] = str;
        if (0 > fork_and_run(args[0], args)) {
            perror("fork_and_run");
            continue;
        }
    }
    if (0 > reap_children())
        FAIL_EXIT("reap_children");
    return 0;
}
