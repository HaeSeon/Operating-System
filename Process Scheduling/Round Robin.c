#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>



int burst_t[100];      //각 프로세스가 완료되기위해 필요한 동작 시간
int arr_t[100];        //각 프로세스의 도착 시간
int wait_t[100];       //각 프로세스의 대기 시간
int total_t[100];      //각 프로세스의 전체 소요시간
int rem_t[100];        //각 프로세스의 남은 동작 시간





//프로세스의 버스트타임과 도착시간을 랜덤으로 생성하고 도착시간순으로 정렬하는 함수
int generate_process(int n) {


	printf("프로세스 번호\t프로세스의 버스트타임\t프로세스의 도착시간\n");
	srand(time(NULL));      //실행할 때마다 다르게 난수를 생성하기 위함
	for (int i = 0; i < n; i++) {
		burst_t[i] = rand() % 50 + 1;    //버스트타임(동작시간) = 1이상 50이하의 수로 구성된 난수 생성
		arr_t[i] = rand() % 50 + 1;      //도착시간=  1이상 50이하의 수로 구성된 난수 생성
		rem_t[i] = burst_t[i];           //처음의 남은시간은 프로세스의 동작시간이므로 대입해준다. 
		//printf("%d \t\t %d \t\t %d\n", i+1, burst_t[i],arr_t[i]);
	}


	//프로세스를 도착순으로 정렬 (insertion sort 알고리즘)
	int i, j, key, temp1, temp2;
	i = 0;
	for (j = 1; j < n; j++) {
		key = arr_t[j];             //현재 숫자, j번째 수를 key값으로 한다.
		temp1 = burst_t[j];
		temp2 = rem_t[j];
		i = j - 1;                //j-1번째부터 역순으로 key갑과 비교 
		while (i >= 0 && arr_t[i] > key) {   //도착시간순으로 정렬할 것이기 때문에 도착시간을 기준으로 비교해야한다. 
			arr_t[i + 1] = arr_t[i];         //도착시간 정렬 (레코드 오른쪽 이동)
			burst_t[i + 1] = burst_t[i];     //동작 시간 정렬 (레코드 오른쪽 이동)
			rem_t[i + 1] = rem_t[i];         //남은 동작시간 정렬 (레코드 오른쪽 이동)
			i = i - 1;
		}
		arr_t[i + 1] = key;
		burst_t[i + 1] = temp1;
		rem_t[i + 1] = temp2;
	}

	for (i = 0; i < n; i++)
		printf("%d \t\t\t %d \t\t\t %d\n", i + 1, burst_t[i], arr_t[i]);          //프로세스 번호 (1부터), 버스타타임, 도착시간 출력


}




//라운드로빈
int RR(int n, int timeQuantum) {
	
	int count = 0;
	int tt = 0; 
	int temp = 0;
	float ava_wait_t=0;     //평균 대기시간
	float ava_total_t=0;    //평균 소요시간
	int i,temp2;
	int j,k = 0;
	int wt = 0;



	//프로세스들을 퀀텀만큼 순서대로 동작시킨다. 
	while (1) {
		for (i = 0, count = 0; i < n; i++) {     //for 문으로 들어온 프로세스 차례대로 연산
			temp = timeQuantum;

			temp2 = 0;
			for (j = k; j < i; j++)
				temp2 = temp2 + burst_t[j];
			wt = temp2 - arr_t[i] + arr_t[k];
			if (wt <= 0)
				k = i;


			if (rem_t[i] == 0) {     
				count++;        //남은 동작시간이 0인 갯수를 카운트함
				//printf("%d  %d  %d\n", i,  n , count);
				continue;
				
			}
			if (rem_t[i] > timeQuantum)
				rem_t[i] = rem_t[i] - timeQuantum;       //프로세스들이 자기 차례가 왔을 때, 퀀텀만큼 동작하는것을 나타냄

			else if (rem_t[i] >= 0) {
				temp = rem_t[i];
				rem_t[i] = 0;                //퀀텀시간보다 남은시간이 작으면 퀀텀내에 모두 작동하므로 남은시간에 0을 넣어줌, 퀀텀이 아닌 남은 시간만큼 시간이 소요됨을 계산해야하므로 temp에 담아줌 
			}
			tt = tt + temp;          //프로세스별 소요시간 계산  
			total_t[i] = tt-arr_t[i]+arr_t[k];         //프로세스별 계산된 소요시간을 배열에 담아줌

			
		//printf("%d , %d, %d, %d, %d\n", i, count,n, rem_t[i], total_t[i]);

		}
	
		
		if (n == count) 
			break;   //남은 동작시간을 카운트한 변수 count가 프로세스개수인 n과 같아진다는 것은 모든 프로세스의 남은 동작시간이 0이라는 의미. =>while문 빠져나옴
	}

	for (i = 0; i < n; i++) {
		wait_t[i] = total_t[i] - burst_t[i];       //프로세스의 대기시간= 소요 시간 - 동작 시간
		ava_wait_t = ava_wait_t + wait_t[i];       //전체 프로세스의 대기시간의 합
		ava_total_t = ava_total_t + total_t[i];    //전체 프로세스의 소요시간의 합
	}

	ava_wait_t = ava_wait_t / n;      //대기시간 평균
	ava_total_t = ava_total_t / n;    //소요시간 평균


	//출력
	printf("프로세스 번호 \t 버스트시간 \t 대기시간\t 소요시간 \n");
	for (i = 0; i < n; i++)
		printf("%d \t\t %d \t\t %d \t\t %d \n", i + 1, burst_t[i], wait_t[i], total_t[i]);
	printf("평균 대기 시간 : %f", ava_wait_t);
	printf("\n평균 소요 시간 : %f", ava_total_t);

}

int main() {
	int n;
	int timeQuantum;
	//입력받기
	printf("100 이하의 프로세서의 개수를 입력하세요.\n");
	scanf("%d", &n);              //100이하의 프로세스 갯수 입력받아 n에 대입
	if (n > 100 || n < 1) {           //100초과, 1미만의 수를 입력했을때 오류처리
		printf("100이하의 수를 입력하세요.");
		exit();            //프로그램 강제종료
	}
	printf("타임퀀텀을 입력하세요.\n");
	scanf("%d", &timeQuantum);          //퀀텀시간 입력받아 timeQuantum에 대입




	generate_process(n);        //n개 프로세스 생성/정렬             
	
	RR(n, timeQuantum);         //라운드로빈

}
