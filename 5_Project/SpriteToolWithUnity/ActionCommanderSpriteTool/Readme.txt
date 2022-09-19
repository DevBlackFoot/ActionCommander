Function: AddForce
String: [X push 값] [Y push 값]
비고 : push 되는 정도는 Unit 단위로 생각했습니다. (프로젝트 기준 PixelPerUnit 1) 

Funcion : Freeze
String: (없음)
비고 : 물리체의 속도를 모두 0으로 만듬. 별도의 지정 속성이 없어서 String 비워두었습니다.

Function: BasicAttack1_Attack1
String : [offset_x] [offset_y] [rect_width] [rect_height]
비고 : Offset은 캐릭터 중심이 기준 이때 값은 Pixel 단위로 생각했습니다.
	Attack에 경우 Attack 별로 별도 처리가 있기때문에 모두 분할되어 있습니다.
	(Ex: BasicAttack2_Attack1, BasicAttack3_Attack1 등)

Function: CameraShake_Rect
String: [offset_x] [offset_y] [rect_width] [rect_height] [X ShakeIntensity] [Y ShakeIntensity] [ShakeTime]
비고: 조건부 발동 카메라 쉐이킹 지정된 영역에 몬스터가 확인되면 발동
	Attack Event와 같이 사용하여 Attack이 성공했을때 발동하는것처럼 사용할 예정
	[offset_x] [offset_y] [rect_width] [rect_height] 부분은 위 Attack Event와 동일
	[X ShakeIntensity] X축 쉐이킹의 강도
	[Y ShakeIntensity] Y축 쉐이킹의 강도
	[ShakeTime] 쉐이킹의 지속시간 (단위: 초)

Function: CameraShake
String: [X ShakeIntensity] [Y ShakeIntensity] [ShakeTime]
비고: 무조건으로 발동시킬 카메라 쉐이킹
	[X ShakeIntensity] X축 쉐이킹의 강도
	[Y ShakeIntensity] Y축 쉐이킹의 강도
	[ShakeTime] 쉐이킹의 지속시간 (단위: 초)

Function: Sound
String: [SoundFile Name]
비고: 사운드파일을 재생한다.
	[SoundFile Name] 사운드 파일에 이름 (확장자 포함)

Function: Effect_Stay
String: [offSet_x] [offset_y] [Effect Animation 경로] 
비고 : 이펙트 애니메이션을 지정된 위치에서 고정되어 실행시킨다.
	[offSet_x] [offset_y] x, y 축 offset -> 부모 오브젝트의 현재 애니메이션 피봇 기준
	[Effect Animation Cilp] 이펙트 애니메이션 json 파일 
	( ex. Assets/Animation/Effect/Effect_Jump.json )

Function: Effect_Follow
String: [X offSet] [Y offset] [Effect Animation 경로] 
비고 : 이펙트 애니메이션을 지정된 위치에서 시작하여 부모 오브젝트의 Transform을 따라오도록 실행시킨다.
	[offSet_x] [offset_y] x, y 축 offset -> 부모 오브젝트의 현재 애니메이션 피봇 기준
	[Effect Animation Cilp] 이펙트 애니메이션 json 파일 
	( ex. Assets/Animation/Effect/Effect_Jump.json )