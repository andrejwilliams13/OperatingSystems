/* timer.c */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{ //signal handler
  printf("Hello World!\n");
  exit(1); //exit after printing
}

int main(int argc, char * argv[])
{
  signal(SIGALRM,handler); //register handler to handle SIGALRM
  alarm(1); //Schedule a SIGALRM for 1 second
  while(1); //busy wait for signal to be delivered
  return 0; //never reached
}

struct msg_requestLock {

        int lockID;   

        int Action

};

struct msg_LockStatus {

        int status;

        int by_child;          

}

struct lock {

        int marked;

        int by_child;

};

int deadlock = NO_DEADLOCK;  

int pid [NPROC];

void finish() {

        int i;

        for(i = 0; i < NPROC; i++)

                kill( pid[i], 9);

        exit(0);

}

void child (int pid, int req, int ack) {

        int rand_lock;       

      int count = 0;       

        struct msg_requestLock MSG;  

        struct msg_LockStatus STAT;   

        struct timeval tt;

        (void) gettimeofday(&tt, NULL);

        srand(tt.tv_sec * 1000000 + tt.tv_usec);

        for(;;) {

                MSG.lockID = rand() % MAXLOCKS;

                MSG.Action = LOCK;

                printf("\tChild %d: Requesting lock %d . . .\n", pid, MSG.lockID);

                fflush(stdout);

                write( req, (char *) &MSG, sizeof(MSG));

                read ( ack, (char *) &STAT, sizeof(STAT));

                if( STAT.status == GRANTED ) {      /* Success got lock */

                        count++;

                        printf("\tChild %d: Got lock %d (%d).\n", pid, MSG.lockID, count);

                        fflush(stdout);

                }

#ifdef TRACE

                if( STAT.status == GRANTED )   

                        printf("\tChild %d: Got lock.\n", pid);

                else if( STAT.status == NOT_GRANTED)

                        printf("\tChild %d: Child %d owns this lock.\n", pid, STAT.by_child);

                else if( STAT.status == YOU_OWN_IT)

                        printf("\tChild %d: I own this lock.\n", pid);

                printf("\tChild %d: Owns %d locks now.\n", pid, count);

                fflush(stdout);

#endif

                if( STAT.status == NOT_GRANTED ) {

                        printf("Child %d waiting for lock %d\n", pid, MSG.lockID);

                        fflush(stdout);

                        read ( ack, (char *) &STAT, sizeof(STAT));

                        if( STAT.status == GRANTED ) {

                                count++;

                                printf("\tChild %d: Got lock %d (%d).\n", pid, MSG.lockID, count);

                        }

                        else if( STAT.status == PREVENT ) {

                                printf("CHILD: %d Will try again, (preventing)\n", pid);

                                fflush(stdout);

                        }

                        else {

                                printf("CHILD: %d    FATAL ERROR\n", pid);

                                fflush(stdout);

                                exit(-1);

                        }

                }

                if( count >= MAXCHILDLOCKS ) {

                        /*

                         * Child sends request to release all its locks

                         */

                        printf("\tChild %d: Requesting RELEASE locks.\n", pid);

                       fflush(stdout);

                        MSG.Action=RELEASE;

                        write(req,(char *) &MSG,sizeof(struct msg_requestLock));

                        count = 0;
sleep(1);

                }

        } /* for(;;) */

} /* child */

int CheckForDeadLock() {

        return(NO_DEADLOCK);

}

int LockManager( int q, struct msg_requestLock ChildRequest, int respond[NPROC] ) {

        int i;

        struct msg_LockStatus STAT;

        int deadlock=NO_DEADLOCK;

        if( ChildRequest.Action == RELEASE ) {

        }

       

        if( ChildRequest.Action == LOCK ) {

                int t_lock;

                t_lock = ChildRequest.lockID;

                if( locks[t_lock].marked == NOTMARKED ) {

                                                    }

                

                        if( locks[t_lock].by_child == q ) {

                                    

                           

                     

                                 

                                STAT.status=YOU_OWN_IT;

                                write(respond[q], (char *) &STAT, sizeof(STAT));

                        }

                        else {

                              

                                STAT.status=NOT_GRANTED;               

                                STAT.by_child = locks[t_lock].by_child;

                                write(respond[q], (char *) &STAT, sizeof(STAT));

                                if( CheckForDeadLock() == DEADLOCK_DETECTED ) {

                                       

                                        printf("NOW ROLLBACK\n");

                                       

                                        

                                        STAT.status=PREVENT;           

                                        write(respond[q], (char *) &STAT, sizeof(STAT));

                                       

                        }

                }

       

        return(deadlock);

}
