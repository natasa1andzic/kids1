#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "domaci1.h"

#define TRUE 1
#define FALSE 0

/*Po MCS algoritmu node ima pokazivac na naredni i
oznaku da li se izvrsava (1 - radi, 0 - ne radi nego ceka)*/
typedef struct mcs_node {
    volatile int blocked;
    volatile struct mcs_node *next;
} mcs_node;

/*Pokazivac poslednjeg node-a ne pokazuje ni na sta, tj. na NULL
Poslednji node se naziva tail*/
volatile mcs_node volatile *tail = NULL;

void getNext(mcs_node *node) {
    mcs_node *next = node->next;
    mcs_node *temp;

    // da se slucajno nije naredni node timeoutovao i odblokirao
    while (!lrk_compare_and_set(&(next->blocked), TRUE, FALSE)) {
        temp = next->next;
        free(next);
        next = temp;
    }
}

int lock_n_threads_with_timeout(int id, int *local, double timeout) {
    double startTime = lrk_get_time();
    mcs_node *node = malloc(sizeof(mcs_node));
    //idemo na kraj reda, pa nas pokazivac pokazuje na null
    //i blokirani smo (blocked=1)
    node->blocked = TRUE;
    node->next = NULL;
    /*da ne bi dva threada setovali tail na sebe,
    kada node pre nas menja svoj pokazivac, da slucajno ne uleti
    neki novi cvor pre nego sto ga prebaci na nas
    get and set je atomicna operacija!*/
    mcs_node *last = lrk_get_and_set(&tail, node);

    if (last != NULL) {
    		// cekam da me odblokira drugi thread
        node->blocked = TRUE;
        last->next = node;
        double timePassed;
        while (node->blocked) {
            timePassed = lrk_get_time() - startTime;
            if (timePassed <= timeout) {
                lrk_sleep(1);
            } else {
                if (!lrk_compare_and_set(&(node->blocked), TRUE, FALSE)) {
                    getNext(node);
                }
                return 0;
            }
        }
    }

    //red je bio prazan i odmah se izvrsavamo
    if (lrk_get_time() - startTime > timeout) {
        getNext(node);
        return 0;
    }
    local[0] = node;
    return 1;
}

void unlock_n_threads_with_timeout(int id, int *local) {
    mcs_node *node = local[0];
    if (node->next == NULL) {
    		// da se nije dodao neki u medjuvremenu pa da zeznemo
    		// compare and set je atomicna operacija!
        if (lrk_compare_and_set(&tail, node, NULL)){
            return;
        }
        while (node->next == NULL)
            ;
    }
    getNext(node);
    free(node);
}

int main() {
    start_timeout_mutex_n_threads_test(0.05);
}


