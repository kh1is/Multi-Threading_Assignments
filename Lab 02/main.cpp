#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

typedef struct BAT{

    string dir;
    int num;

}BAT;


class monitor{



   public:
		int NorthCounter;
        int EastCounter;
        int SouthCounter;
        int WestCounter;



        pthread_mutex_t mutex;

		pthread_mutex_t mutex2;

		pthread_mutex_t mutexN;
		pthread_mutex_t mutexE;
		pthread_mutex_t mutexS;
		pthread_mutex_t mutexW;

		pthread_mutex_t mutex3;
		pthread_mutex_t mutex4;
		pthread_mutex_t mutex5;

        pthread_cond_t NorthQueue;
        pthread_cond_t EastQueue;
        pthread_cond_t SouthQueue;
        pthread_cond_t WestQueue;
        pthread_cond_t NorthFirst;
        pthread_cond_t EastFirst;
        pthread_cond_t SouthFirst;
        pthread_cond_t WestFirst;


    void arrive(BAT b){



		pthread_mutex_lock(&mutex3);

        cout << "BAT " << b.num << " from " << b.dir << " arrives at crossing " << endl;

		pthread_mutex_unlock(&mutex3);


		if(b.dir == "n"){
		    pthread_mutex_lock(&mutexN);
	        NorthCounter++;
	        if(NorthCounter > 1){
	            pthread_cond_wait(&NorthQueue,&mutexN);
	        }
		    pthread_mutex_unlock(&mutexN);
	        cross(b);

	    }

        else if(b.dir == "e"){

	    	pthread_mutex_lock(&mutexE);
            EastCounter++;
            if(EastCounter > 1){
                pthread_cond_wait(&EastQueue,&mutexE);
            }
	    	pthread_mutex_unlock(&mutexE);
            cross(b);


        }
        else if(b.dir == "s"){
	    	pthread_mutex_lock(&mutexS);
            SouthCounter++;
            if(SouthCounter > 1){
                pthread_cond_wait(&SouthQueue,&mutexS);
            }
	    	pthread_mutex_unlock(&mutexS);
            cross(b);


        }
        else if(b.dir == "w"){
	    	pthread_mutex_lock(&mutexW);
            WestCounter++;
            if(WestCounter > 1){
                pthread_cond_wait(&WestQueue,&mutexW);
            }
	    	pthread_mutex_unlock(&mutexW);
            cross(b);


        }



    }

    void cross(BAT b){

    	check();

		pthread_mutex_lock(&mutex);



		if(b.dir == "n"){

            if(WestCounter > 0){
               pthread_cond_wait(&WestFirst,&mutex);


            }
 			NorthCounter--;
 			pthread_mutex_lock(&mutex4);

			cout << "BAT " << b.num << " from " << b.dir << " crossing " << endl;

			pthread_mutex_unlock(&mutex4);

        	sleep(1);

			leave(b);
	    	if(NorthCounter>0){
            	pthread_cond_signal(&NorthQueue);

	    	}



        }
        else if(b.dir == "e"){

            if(NorthCounter > 0){

                pthread_cond_wait(&NorthFirst,&mutex);

            }
			EastCounter--;
			pthread_mutex_lock(&mutex4);

			cout << "BAT " << b.num << " from " << b.dir << " crossing " << endl;

			pthread_mutex_unlock(&mutex4);

        	sleep(1);

			leave(b);
            if(EastCounter>0){
            	pthread_cond_signal(&EastQueue);
	    	}


        }
        else if(b.dir == "s"){

            if(EastCounter > 0){
                pthread_cond_wait(&EastFirst,&mutex);
            }
			SouthCounter--;
			pthread_mutex_lock(&mutex4);

			cout << "BAT " << b.num << " from " << b.dir << " crossing " << endl;

			pthread_mutex_unlock(&mutex4);

        	sleep(1);

			leave(b);
	    	if(SouthCounter>0){
				pthread_cond_signal(&SouthQueue);
	    	}


        }
        else if(b.dir == "w"){

            if(SouthCounter > 0){
                pthread_cond_wait(&SouthFirst,&mutex);
            }
	    	WestCounter--;
	    	pthread_mutex_lock(&mutex4);

			cout << "BAT " << b.num << " from " << b.dir << " crossing " << endl;

			pthread_mutex_unlock(&mutex4);

        	sleep(1);

			leave(b);
	    	if(WestCounter>0){
				pthread_cond_signal(&WestQueue);
	    	}



        }



	// pthread_mutex_lock(&mutex4);

	// cout << "BAT " << b.num << " from " << b.dir << " crossing " << endl;

	// pthread_mutex_unlock(&mutex4);

 //        sleep(1);

	// leave(b);
	//check();

    }

    void leave(BAT b){



		pthread_mutex_lock(&mutex5);


        cout << "BAT " << b.num << " from " << b.dir << " leaving crossing " << endl;


		pthread_mutex_unlock(&mutex5);



        if(b.dir == "n"){


            if(EastCounter > 0){

               	pthread_cond_signal(&NorthFirst);

            }
            else if(NorthCounter > 0){
                pthread_cond_signal(&WestFirst);
            }

        }
        else if(b.dir == "e"){


            if(SouthCounter > 0){
                pthread_cond_signal(&EastFirst);
            }
            else if(EastCounter > 0){
                pthread_cond_signal(&NorthFirst);
            }

        }
        else if(b.dir == "s"){


            if(WestCounter > 0){
                pthread_cond_signal(&SouthFirst);
            }
            else if(SouthCounter > 0){
                pthread_cond_signal(&EastFirst);
            }
        }
        else if(b.dir == "w"){


            if(NorthCounter > 0){
                pthread_cond_signal(&WestFirst);
            }
            else if(WestCounter > 0){
                pthread_cond_signal(&SouthFirst);
            }

        }

		pthread_mutex_unlock(&mutex);



    }

    void check(){

		if(NorthCounter > 0 && WestCounter > 0 && SouthCounter > 0 && EastCounter > 0){
			cout << "deadlock is occured" << endl;

			//pthread_cond_signal(&WestFirst);
			pthread_mutex_unlock(&mutex);


		}


    }



};

monitor m;


void* newBAT(void* pars){

    BAT *temp = (BAT*)pars;

    BAT b;
    b.num = temp->num;
    b.dir = temp->dir;

	m.arrive(b);



}


int main()
{


    string s;
    getline(cin,s);


    int l = s.length();

    pthread_t tid[l];
    BAT b[l];

    int i;

    pthread_mutex_init(&m.mutex, NULL);

    pthread_mutex_init(&m.mutex2, NULL);

    pthread_mutex_init(&m.mutexN, NULL);
    pthread_mutex_init(&m.mutexE, NULL);
    pthread_mutex_init(&m.mutexS, NULL);
    pthread_mutex_init(&m.mutexW, NULL);


    pthread_mutex_init(&m.mutex3, NULL);
    pthread_mutex_init(&m.mutex4, NULL);
    pthread_mutex_init(&m.mutex5, NULL);

    pthread_cond_init(&m.NorthQueue,NULL);
    pthread_cond_init(&m.EastQueue,NULL);
    pthread_cond_init(&m.SouthQueue,NULL);
    pthread_cond_init(&m.WestQueue,NULL);
    pthread_cond_init(&m.NorthFirst,NULL);
    pthread_cond_init(&m.EastFirst,NULL);
    pthread_cond_init(&m.SouthFirst,NULL);
    pthread_cond_init(&m.WestFirst,NULL);

    for(i=0;i<l;i++){

        b[i].dir = s[i];
        b[i].num = i+1;


       int ret = pthread_create(&tid[i],NULL,newBAT,(void*)&b[i]);

       if(ret != 0){
            cout << "error" << endl;
       }
    }

    for(i=0;i<l;i++){

        pthread_join(tid[i],NULL);

    }


    pthread_mutex_destroy(&m.mutex);

    pthread_mutex_destroy(&m.mutex2);

    pthread_mutex_destroy(&m.mutexN);
    pthread_mutex_destroy(&m.mutexE);
    pthread_mutex_destroy(&m.mutexS);
    pthread_mutex_destroy(&m.mutexW);

    pthread_mutex_destroy(&m.mutex3);
    pthread_mutex_destroy(&m.mutex4);
    pthread_mutex_destroy(&m.mutex5);


    pthread_cond_destroy(&m.NorthQueue);
    pthread_cond_destroy(&m.EastQueue);
    pthread_cond_destroy(&m.SouthQueue);
    pthread_cond_destroy(&m.WestQueue);
    pthread_cond_destroy(&m.NorthFirst);
    pthread_cond_destroy(&m.EastFirst);
    pthread_cond_destroy(&m.SouthFirst);
    pthread_cond_destroy(&m.WestFirst);

    return 0;
}

