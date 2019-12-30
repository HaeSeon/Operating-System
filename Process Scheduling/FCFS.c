#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>




int burst_t[100];      //각 프로세스가 완료되기위해 필요한 동작 시간
int arr_t[100];        //각 프로세스의 도착 시간
int wait_t[100];       //각 프로세스의 대기 시간
int total_t[100];      //각 프로세스의 전체 소요시간






//프로세스의 버스트타임과 도착시간을 랜덤으로 생성하고 도착시간순으로 정렬하는 함수
int generate_process(int n) {


	printf("프로세서 번호\t프로세스의 버스트타임\t프로세스의 도착시간\n");
	srand(time(NULL));     //실행할 때마다 다르게 난수를 생성하기 위함
	for (int i = 0; i < n; i++) {
		burst_t[i] = rand() % 50 + 1;    //버스트타임(동작시간) = 1이상 50이하의 수로 구성된 난수 생성
		arr_t[i] = rand() % 50 + 1;      //도착시간=  1이상 50이하의 수로 구성된 난수 생성

		//printf("%d \t\t %d \t\t %d\n", i+1, burst_t[i],arr_t[i]);
	}


	//프로세스를 도착순으로 정렬 (insertion sort 알고리즘)
	int i, j, key, temp1;
	i = 0;
	for (j = 1; j < n; j++) {
		key = arr_t[j];     //현재 숫자, j번째 수를 key값으로 한다. 
		temp1 = burst_t[j];
		i = j - 1;            //j-1번째부터 역순으로 key갑과 비교 
		while (i >= 0 && arr_t[i] > key) {       //도착시간순으로 정렬할 것이기 때문에 도착시간을 기준으로 비교해야한다.
			arr_t[i + 1] = arr_t[i];             //도착시간 정렬 (레코드 오른쪽 이동)
			burst_t[i + 1] = burst_t[i];         //동작 시간 정렬(레코드 오른쪽 이동)
			i = i - 1;
		}
		arr_t[i + 1] = key;
		burst_t[i + 1] = temp1;
	}

	for (i = 0; i < n; i++)
		printf("%d \t\t\t %d \t\t\t %d\n", i + 1, burst_t[i], arr_t[i]);	//프로세스 번호 (1부터), 버스타타임, 도착시간 출력


}


//FCFS
int FCFS(int n) {

	int wt = 0;        //대기시간을 구하기위한 변수
	int temp;          //프로세스들의 버스트타임 합을 구하기 위한 변수 (대기시간을 구하기 위해 씀)  
	float ava_wait_t = 0;          //평균 대기시간
	float ava_total_t = 0;         //평균 소요시간
	int i;
	int j, k = 0;



	for (i = 0; i < n; i++) {
		temp = 0;
		for (j = k; j < i; j++) 
			temp = temp + burst_t[j];     //프로세스들의 버스트타임의 합
			
		
		wt = temp - arr_t[i] + arr_t[k];      //대기시간 = 이전 프로세스들의 버스트타임 합 - 현재 프로세스의 도착시간 + 시작지점기준 프로세스 도착시간 
		//printf("%d  %d  %d\n", i, arr_t[j], arr_t[k]);
		
		if (wt <= 0) { 
			k = i;            //들어온 프로세스의 대기시간이 없을 때 시작지점기준을 해당 프로세스로 한다. 이 경우의 대기시간은 0이다. 
			total_t[i] = burst_t[i] + wait_t[i];       //소요시간 구하기 
			//printf("%d  %d  %d\n", i, j, k);
		}
		else {
			wait_t[i] = wt;        //각 프로세스의 대기시간 배열에 저장
			total_t[i] = burst_t[i] + wait_t[i];       //소요시간 구하기

		}

		
	}




	for (i = 0; i < n; i++) {
		ava_wait_t = ava_wait_t + wait_t[i];         //전체 프로세스의 대기시간의 합
		ava_total_t = ava_total_t + total_t[i];      //전체 프로세스의 소요시간의 합
	}

	ava_wait_t = ava_wait_t / n;        //대기시간 평균
	ava_total_t = ava_total_t / n;      //소요시간 평균

	//출력
	printf("프로세스 번호 \t 버스트시간 \t 대기시간\t 소요시간 \n");
	for (i = 0; i < n; i++)
		printf("%d \t\t %d \t\t %d \t\t %d \n", i + 1, burst_t[i], wait_t[i], total_t[i]);
	printf("평균 대기 시간 : %f", ava_wait_t);
	printf("\n평균 소요 시간 : %f", ava_total_t);

}

int main() {

	int n;
	//입력받기
	printf("100 이하의 프로세서의 개수를 입력하세요.\n");
	scanf("%d", &n);                 //100이하의 프로세스 갯수 입력받아 n에 대입

	if (n > 100 || n < 1) {         //100초과, 1미만의 수를 입력했을때 오류처리
		printf("100이하의 수를 입력하세요.");
		exit();       //프로그램 강제종료
	}

	generate_process(n);             //n개 프로세스 생성/정렬

	FCFS(n);                         //FCFS

}
