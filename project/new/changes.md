# 피드백 받은 거

- 굳이굳이 HTML 만들어서 브라우저로 키진 마라
- 스펙 가져온 걸로 특정 게임 실행 가능하게 알려주는 건 어떨까?
  - 그냥 PC Building Simulator의 실행 가능 각? 그 프로그램 만들면 될 듯
- 서버-클라이언트 구조 사용 가능
- 프로젝트 언어를 C에서 바꾸지는 마라
  - 서버까지 C로 해야 하나? 일단 go로 만들고 안 되면 나중에 바꾸는 걸로

# 어떤 게임들?
- Minecaft JE
- PUBG
- Stardew Valley

# 산출 방식
- 램은 대역폭 생각 안 하고 용량만 크냐 작냐로 비교
- CPU/GPU는 3D Mark의 TimeSpy 참조
    - API(https://www.3dmark.com/ 뜯기)
|Endpoint|역할|Params|응답|비고|
|---|---|---|---|---|
|/cpuname|CPU이름으로 ID 가져오기|term={검색할 이름}|[{id: ID, label: CPU 이름, label: CPU 이름},...]||
|/gpuname|GPU이름으로 ID 가져오기|동일|동일||
|/newsearch|벤치마크 결과 검색|test=spy P&cpuId={CPU ID}&gpuId={GPU ID}&gpuCount=0&gpuType=ALL&deviceType=ALL&storageModel=ALL&showRamDisks=false&memoryChannels=0&country=&scoreType=overallScore&hofMode=false&showInvalidResults=false&freeParams=&minGpuCoreClock=&maxGpuCoreClock=&minGpuMemClock=&maxGpuMemClock=&minCpuClock=&maxCpuClock=|{results:[{resultId: 결과 ID, rank: 순위, test: 테스트한 벤치마크, gpuCount: GPU 개수, date: 테스트 일시(string), cpu: cpu이름, gpu: gpu이름, country: 국가, overallScore: 전체 점수, graphicScore: 그래픽 스코어, physicsScore: CPU 스코어}]}|cpuId, gpuId 없어도 가져올 수 있음|

# API(C에서 응답을 주고 받아야 하니 최대한 단순하게)
- 모든 요청은 GET으로
|endpoint|역할|Params|응답|비고|
|---|---|---|---|---|
|/check|게임이 실행 가능한지 판별|cpu=CPU 이름, gpu=GPU 이름, game=Game ID|요청 성공(0,1),실행가능(0,1),최소CPU(string),최소GPU(string),최소CPU스코어 대비 내 CPU 스코어(int),최소GPU스코어 대비 내 GPU 스코어(int)|최소 스코어들은 실행이 불가능 할 때만 %로 표시|
|/list|확인 가능한 게임 리스트 가져오기||게임ID(int),게임이름(string)\n게임ID,게임이름...|