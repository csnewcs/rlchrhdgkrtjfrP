//CPU 이름 가져오기: /proc/cpuinfo의 model name 부분
//메모리 용량 가져오기: free --giga -h 에서 Mem: 부분 중 첫 번째(total)
//GPU 이름 가져오기: lshw -C display에서 product 부분"들" 중 대괄호 안 (바깥은 GPU 코어 이름, NAVI14와 같은)