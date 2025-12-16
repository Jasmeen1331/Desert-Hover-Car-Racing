// Car Race.cpp: A program using the TL-Engine

#include "TL-Engine11.h" 
using namespace tle;

//Creating structures
struct Vector3D
{
	float x;
	float y;
	float z;
};
Vector3D scalar(float s, Vector3D v)
{
	return{ s * v.x, s * v.y, s * v.z };
}
Vector3D addThree(Vector3D v1, Vector3D v2, Vector3D v3)
{
	return{ v1.x + v2.x + v3.x, v1.y + v2.y + v3.z,v1.z + v2.z + v3.z };
}
float vector3Dm(Vector3D vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

//Turning Car Angle
const float PI = 3.14159f;

Vector3D GetVector2DFromAngle(float angle)
{
	Vector3D result;
	result.x = sin(angle * (PI / 180));
	result.z = cos(angle * (PI / 180));
	return result;
}

int main()
{
	// Create a 3D engine (using TL11 engine here) and open a window for it
	TLEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\Users\\HP-LAPTOP\\Desktop\\GG\\Media");

	/**** Set up your scene here ****/

	//Assigning the coordinates for sky, ground, car
	const Vector3D skyboxcoord = { 0.0f,-960.0f,0.0f };
	const Vector3D hover_carcoord = { 0.0f,0.0f,-30.0f };
	const Vector3D camcoord = { 0.0f,20.0f, -80.0f };

	// Craeting Models
	IMesh* SkyboxMesh = myEngine->LoadMesh("Skybox 07.x");
	IModel* Skybox = SkyboxMesh->CreateModel(skyboxcoord.x, skyboxcoord.y, skyboxcoord.z);

	IMesh* groundMesh = myEngine->LoadMesh("ground.x");
	IModel* ground = groundMesh->CreateModel();

	IMesh* hover_carMesh = myEngine->LoadMesh("race2.x");
	IModel* hover_car = hover_carMesh->CreateModel(hover_carcoord.x, hover_carcoord.y, hover_carcoord.z);
	IModel* nplayercar = hover_carMesh->CreateModel(hover_carcoord.x+10, hover_carcoord.y, hover_carcoord.z);
	nplayercar->SetSkin("sp01.jpg");

	IMesh* crossMesh = myEngine->LoadMesh("Cross.x");
	IModel* cross;

	ICamera* camera = myEngine->CreateCamera(kManual, camcoord.x, camcoord.y, camcoord.z);


	//Writing FOnt in dialoug box
	IFont* myfont = myEngine->LoadFont("Times New Roamn", 80);
	IFont* myfont2 = myEngine->LoadFont("Arial", 50);
	ISprite* backdrop = myEngine->CreateSprite("ui_backdrop.jpg", 0.0f, 0.0f);
	ISprite* backdrop1 = myEngine->CreateSprite("RedGlow.jpg", 5.0f, 60.0f);
	
	
	// Calling out the functions
	float distance(IModel * checkpointDummy, IModel * hover_car);
	float hover_car2checkpoint(IModel * checkpointDummy, IModel * hover_car, float hover_carmodel, float checkpointradius);
	float hover_cartowall(IModel * hover_car, IModel * wall, float wallWidth, float hover_carmodel, float walldepth);
	float hover_cartotank(IModel * tank, IModel * hover_car, float tanklength, float tankRadius, float tankdepth);
	float hover_cartonplayer(IModel * hover_car, IModel * nplayercar, float hover_carmodel, float hover_cardepth);
	float hover_cartotribune(IModel * tribune, IModel * hover_car, float tribunelength, float tribuneRadius, float tribunedepth);
	float hover_cartoisle(IModel * hover_car, IModel * isle, float isle_length, float hover_carmodel, float isleDepth);

	//Calculating movement of car
	Vector3D momentum = {};
	Vector3D thrust = {};
	Vector3D drag = {};
	Vector3D facingVector = { 0.0f,0.0f,1.0f };

		
	//Declaring all variables
	bool         gamestart = false ;
	bool         game = false;
	bool         lastCheckpointPassed = false;
	bool         chaseCamActive = false;
	bool         mouseCaptureActive = true;
	bool         BoostActive = false;
	bool         Boost_Over_Heat = false;
	bool         Boost_Cool_Down = false;
	float        matrix[4][4];
	float        BoostTimer = 0.0f;
	float        countdownTime = 3.0f;
	float        elapsedTime = 0.0f;
	float        carhitpoint = 0;
	float        carhealthpoints = 100;
	float		 carAngle = 0;
	float        thurstfactor = 6.0f;
	float        BoostCooldown = 5.0f;
	float        cofficientofDrag = -0.03;
	int          stagesComplete = 0;
	int          currentWaypointIndex = 0;
	int          mouseMoveX = 0;
	int          mouseMoveY = 0;
	const int    numberOfStages = 3;
	const int    NumberOfCheckpoint = 4;
	const int    NumberOfwalls = 34;
	const int    NumberOftanks = 3;
	const int    NumberOfwaypoint = 6;
	const int    hover_cardepth = 2.0f;
	const int    NumberOfisle = 35;
	const int    NumberOfFlare = 6;
	const int    NumberOftribune = 5;
	const float  tribunelength=5.0f;
	const float  tribuneRadius=4.0f; 
	const float  tribunedepth = 1.0f;
	const float  tankdepth = 5.0f; 
	const float  tanklength = 8.0f; 
	const float  tankRadius = 2.0f;
	const float  carspeed = 50.0f;
	const float  cameraspeed = 80.0f;
	const float  wallWidth = 5.0f;
	const float  wallDepth = 3.0f;
	const float  isle_length = 4.0f;
	const float  isleDepth = 3.0f;
	const float  BoostDuration = 3.0f;
	const float  BoostOverheatWarningTime = 1.0f;
	const float  BoostThrustFactor = 5.0f;
	const float  turnspeed = 50;
	const float  car_thrust = 100;
	const float  speedrecord = 0.1f;
	const float  hover_carmodel = 5.0f;
	const float  checkpointradius = 0.8f;
	const float  kCameraMove = carspeed * 0.001f;
	const float  kMouseWheelMove = carspeed * 0.001f;
	const float  kMouseRotation = carspeed * 0.003f;
	

	myEngine->StartMouseCapture();
	

	// Array to track completion of each stage
	enum GamesStates { Start, Countdown, Ready };
	GamesStates gameState = Start;

	//Checkpoint setup with dummy models for collision
	IMesh* dummyMesh = myEngine->LoadMesh("Tanksmall2.x");

	IMesh* checkpointMesh = myEngine->LoadMesh("Checkpoint.x");
	Vector3D checkpointpositions[NumberOfCheckpoint] = { {0.0f,0.0f,0.0f},{15.0f,0.0f,120.0f},{50.0f,0.0f,200.0f},{20.0f,0.0f,280.0f} };
	IModel* checkpoint[NumberOfCheckpoint];
	IModel* checkpointDummy[NumberOfCheckpoint * 2];
	
	for (int i = 0; i < NumberOfCheckpoint; i++)
	{
		checkpoint[i] = checkpointMesh->CreateModel(checkpointpositions[i].x, checkpointpositions[i].y, checkpointpositions[i].z);
		checkpointDummy[i * 2] = dummyMesh->CreateModel(checkpointpositions[i].x, checkpointpositions[i].y, checkpointpositions[i].z);
		checkpointDummy[i * 2 + 1] = dummyMesh->CreateModel(checkpointpositions[i].x, checkpointpositions[i].y, checkpointpositions[i].z);
		if (i == 1 || i == 3)
		{
			checkpoint[i]->RotateLocalY(90);
			checkpointDummy[i * 2]->RotateLocalY(90);
			checkpointDummy[i * 2 + 1]->RotateLocalY(90);
		}
		checkpointDummy[i * 2]->MoveLocalX(-10);
		checkpointDummy[i * 2 + 1]->MoveLocalX(10);
		checkpointDummy[i * 2]->Scale(0.0001f);
		checkpointDummy[i * 2 + 1]->Scale(0.0001f);
	}

	//Setup for tribune models
	Vector3D tribunepositions[NumberOftribune] = { {20,0,260},{20,0,95},{83,0,290},{-20,0,120},{20,0,300} };
	IMesh* tribuneMesh = myEngine->LoadMesh("Tribune1.x");
	IModel* tribune[NumberOftribune];
	for (int i = 0; i < NumberOftribune; i++)
	{
		tribune[i] = tribuneMesh->CreateModel(tribunepositions[i].x, tribunepositions[i].y, tribunepositions[i].z);
	}

	//Setup for waypoints to track npalyercar
	Vector3D waypointpositions[NumberOfwaypoint] = { {0,0,50},{0,0,90}, {50,0,150},{50,0,170} ,{50,0,280},{15,0,280} };
	IMesh* waypointMesh = myEngine->LoadMesh("Walkway.x");
	IModel* waypoint[NumberOfwaypoint];
	for (int i = 0; i < NumberOfwaypoint; i++)
	{
		waypoint[i] = waypointMesh->CreateModel(waypointpositions[i].x, waypointpositions[i].y, waypointpositions[i].z);
	}

	//Setup for tanks
	Vector3D tankpositions[NumberOftanks] = { {0,0,70},{50,0,170},{50,0,250} };
	IMesh* tankMesh = myEngine->LoadMesh("TankSmall1.x");
	IModel* tank[NumberOftanks];
	for (int i = 0; i < NumberOftanks; i++)
	{
		tank[i] = tankMesh->CreateModel(tankpositions[i].x, tankpositions[i].y, tankpositions[i].z);
		tank[i]->RotateX(45.0f);
		tank[i]->MoveLocalY(-4.0f);
	}

	//Arranging walls setup
	Vector3D wallpositions[NumberOfwalls] = { {20,0,64},{20,0,80},{20,0,96},{44,0,104},{60,0,104},{76,0,104},{28,0,104},{44, 0, 300}, {60, 0, 300}, {75, 0,300},{-12, 0, 136}, {-4, 0, 136}, {4, 0,136},{-20,0,64},{-20,0,80},{-20,0,96},
	{20, 0, 240} ,{20, 0, 224} ,{20, 0, 208} ,{20, 0, 192},{20, 0, 176}, {20, 0, 160}, {20, 0,144},{84,0,112},
	{83, 0, 128},{83, 0, 144},{83, 0, 160},{83, 0, 176},{83, 0, 192},{83, 0, 208},{83, 0, 224},{83, 0, 240},{83, 0, 256},{83, 0, 272} };
	IMesh* wallMesh = myEngine->LoadMesh("Wall.x");
	IModel* wall[NumberOfwalls];
	for (int i = 0; i < NumberOfwalls; i++)
	{
		wall[i] = wallMesh->CreateModel(wallpositions[i].x, wallpositions[i].y, wallpositions[i].z);
		if (i>=3 && i <= 12)
		{
			wall[i]->RotateLocalY(90.0f);
		}
	}
	//Isle setup
	Vector3D islepositions_1[NumberOfisle] = { {20, 0, 56}, {20, 0, 72},{20, 0, 88}, {20, 0, 104} , {36, 0, 104}, {52, 0, 104}, {68, 0, 104},{84, 0, 104},
	 {36, 0, 300}, {52, 0, 300}, {68, 0,300}, {84, 0, 300},{-20, 0, 136}, {-4, 0, 136}, {12, 0,136}, {12, 0, 136},
		{20, 0, 248} ,{20, 0, 232} ,{20, 0, 216} ,{20, 0, 200} ,{20, 0, 184},{20, 0, 168}, {20, 0, 152}, {20, 0,136},{83, 0, 120},{83, 0, 136},{83, 0, 152},{83, 0, 168},{83, 0, 184},{83, 0, 200},{83, 0, 216},{83, 0, 232},{83, 0, 248},{83, 0, 264},{83, 0, 280},

	};
	IMesh* isle_1Mesh = myEngine->LoadMesh("IsleStraight.x");
	IModel* isle_1[NumberOfisle];
	for (int i = 0; i < NumberOfisle; i++)
	{
		isle_1[i] = isle_1Mesh->CreateModel(islepositions_1[i].x, islepositions_1[i].y, islepositions_1[i].z);
		if (i >= 0 && i <= 3)
		{
			isle_1[i] = isle_1Mesh->CreateModel(islepositions_1[i].x, islepositions_1[i].y, islepositions_1[i].z);
			isle_1[i] = isle_1Mesh->CreateModel(-islepositions_1[i].x, islepositions_1[i].y, islepositions_1[i].z);
		}
	}


	//Chasing the char
	camera->AttachToParent(hover_car);
	myEngine->Timer();

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())

	{
		myEngine->DrawScene();

		//Using frametime throughout the code
		float frameTime = myEngine->Timer();

		// Draw the scene
		if (gamestart=true)
		{
			//begnning with the countdown to start the game
			if (gameState == Start)
			{
				myfont->Draw("Press Space to Start", 750, 300, kBlack, CentreAlign);
				if (myEngine->KeyHit(Key_Space))
				{
					gameState = Countdown;
				}
			}
			if (gameState == Countdown)
			{
				countdownTime -= frameTime;
				if (countdownTime > 2)
					myfont->Draw("3", 750, 300, kLightGrey, CentreAlign);
				else if (countdownTime > 1)
					myfont->Draw("2", 750, 300, kLightGrey, CentreAlign);
				else if (countdownTime > 0)
					myfont->Draw("1", 750, 300, kLightGrey, CentreAlign);
				else if (countdownTime > -0.5f)
				{
					myfont->Draw("Go", 750, 300, kLightGrey, CentreAlign);
					game = true;
				}
				else
					gameState = Ready;
			}
			if (gameState == Ready)
			{
				elapsedTime += frameTime;
				myfont2->Draw("Time: " + std::to_string(lroundf(elapsedTime * 100 / 100)), 0, 70, kDarkGrey);

			}
		}
		/**** Update your scene each frame here ****/
		if (game == true)

		{
			//bobbling effect
			float bobbleAmplitude = 0.5f;
			float bobbleFrequency = 5.0f;
			float bobbleHeight = bobbleAmplitude * sin(2 * PI * bobbleFrequency * frameTime);
			hover_car->SetY(bobbleHeight);


			//Sphere to Sphere collision with gcar and tribune
			for (int i = 0; i < NumberOftribune; ++i)
			{
				if (hover_cartotribune(tribune[i], hover_car, tribunelength, tribuneRadius, tribunedepth))
				{
					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
					carhitpoint++;
					carhealthpoints--;
				}
			}

			//Sphere to box collision with gcar and nplayercar
			if (hover_cartonplayer(hover_car, nplayercar, hover_carmodel, hover_cardepth))
			{
				momentum.x = -momentum.x;
				momentum.z = -momentum.z;
				carhitpoint++;
				carhealthpoints--;

			}
			//Sphere to box collision with gcar and isle
			for (int i = 0; i < NumberOfisle; ++i)
			{
				if (hover_cartoisle(hover_car, isle_1[i], isle_length, hover_carmodel, isleDepth))
				{
					   momentum.x = -momentum.x;
						momentum.z = -momentum.z;
						carhitpoint++;
						carhealthpoints--;

				}
			}

			//Sphere to box Collison with walls 
			for (int i = 0; i < NumberOfwalls; ++i)
			{
				if (hover_cartowall(hover_car, wall[i], wallWidth, hover_carmodel, wallDepth))
				{
					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
					carhitpoint++;
					carhealthpoints--;
				}
			}
			//Sphere to Sphere collision with tanks
			for (int i = 0; i < NumberOftanks; ++i)
			{
				if (hover_cartotank(tank[i], hover_car, tanklength, tankRadius, tankdepth))
				{
					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
					carhitpoint++;
					carhealthpoints--;
				}
			}
			//Sphere to Box collision with checkpoints
			for (int i = 0; i < NumberOfCheckpoint; ++i)
			{

				if (hover_car2checkpoint(checkpointDummy[i * 2], hover_car, hover_carmodel, checkpointradius) ||
					hover_car2checkpoint(checkpointDummy[i * 2 + 1], hover_car, hover_carmodel, checkpointradius))
				{
					momentum.x = -momentum.x;
					momentum.z = -momentum.z;
					carhitpoint++;
					carhealthpoints--;

				}

				//Implementing different satges and conditions
				if (stagesComplete == i && hover_car2checkpoint(checkpoint[i], hover_car, hover_carmodel, checkpointradius))
				{
					stagesComplete++;
					cross = crossMesh->CreateModel(0.0f, 7.0f, 0.0f);
					cross->Scale(0.5f);

				}
				//Car Health Points 
				myfont2->Draw("Car Health Points " + std::to_string(carhealthpoints), 0, 0, kBlue);
				if (carhealthpoints <= 0.0f)
				{
					myfont2->Draw("Race Over", 750, 300, kBlack, CentreAlign);
					myfont2->Draw("Press R to Restart", 750, 250, kBlack, CentreAlign);
					

				}
				if (carhealthpoints < 30.0f) {
					// Health is below 30%, boost cannot operate
					BoostActive = false;
				}
			}
			//Conditions for stage completion

			if (stagesComplete > 0)
			{
				if (stagesComplete == 1)
				{
					myfont2->Draw("Stage 1\nCompleted", 0, 190, kDarkGrey);
				}
				else if (stagesComplete == 2)
				{
					myfont2->Draw("Stage 2\nCompleted", 0, 190, kDarkGrey);
				}
				else if (stagesComplete == 3)
				{
					myfont2->Draw("Stage 3\nCompleted", 0, 190, kDarkGrey);
				}
				else if (stagesComplete == 4)
				{
					myfont2->Draw("Race Over ", 0, 230, kDarkGrey);
					myfont2->Draw("Press R to restart ", 750, 200, kBlack, CentreAlign);
					
				}

				
			}

			

		   //Calculating thrust car movements
			hover_car->GetMatrix(&matrix[0][0]);
			facingVector = { matrix[2][0],matrix[2][1], matrix[2][2] };

			//Using speedometer to get speed
			float speed = vector3Dm(momentum) * speedrecord * 3.6f;
			myfont2->Draw("Speed:" + std::to_string(static_cast<int>(speed)) + "m/sec", 0, 130, kDarkGrey);


			drag = scalar(cofficientofDrag, momentum);
			momentum = addThree(momentum, drag, thrust);

			//Movement of car
			hover_car->Move(momentum.x * frameTime, 0.0f, momentum.z * frameTime);
			thrust = {0,0};


			//Control Keys using W,A,S,D 
			if (myEngine->KeyHeld(Key_W))
			{
				thrust = scalar(thurstfactor, facingVector);
				carAngle -= carspeed * frameTime;
			}
			if (myEngine->KeyHeld(Key_S))
			{
				thrust = scalar(-thurstfactor, facingVector);
				carAngle += carspeed * frameTime;
			}
			if (myEngine->KeyHeld(Key_A))
			{
				hover_car->RotateY(-carspeed * frameTime);
				carAngle -= carspeed * frameTime;
			}
			if (myEngine->KeyHeld(Key_D))
			{
				hover_car->RotateY(carspeed * frameTime);
				carAngle += carspeed * frameTime;
			}
			//R key for Restart the game
			if (myEngine->KeyHeld(Key_R))
			{
				gamestart = true;
				elapsedTime = 0.0f;
				carhitpoint = 0;
				carhealthpoints = 100;
				BoostActive = false;
				Boost_Over_Heat = false;
				BoostTimer = 0.0f;
				countdownTime = 3.0f;
				stagesComplete = 0;
				currentWaypointIndex = 0;
				hover_car->SetPosition(hover_carcoord.x, hover_carcoord.y, hover_carcoord.z);
			}
			

			//Movement of nplayer car
			Vector3D currentWaypoint = waypointpositions[currentWaypointIndex];
			float dx = currentWaypoint.x - nplayercar->GetX();
			float dz = currentWaypoint.z - nplayercar->GetZ();
			float distanceToWaypoint = sqrt(dx * dx + dz * dz);

			if (distanceToWaypoint <= 1.0f)
			{ 
				currentWaypointIndex = (currentWaypointIndex + 1) % NumberOfwaypoint;
			}
			Vector3D direction = { dx, 0.0f, dz };
			float length = vector3Dm(direction);
			if (length > 0.0f) 
			{
				direction.x /= length;
				direction.z /= length;
				// Move the non-player car towards the current waypoint
				nplayercar->LookAt(currentWaypoint.x, nplayercar->GetY(), currentWaypoint.z);
				nplayercar->Move(direction.x * 0.2f, 0.0f, direction.z * 0.2f);
			}
			nplayercar->ResetScale();


			//Implemnting Boost
			if (myEngine->KeyHeld(Key_Space) && !BoostActive && !Boost_Over_Heat) 
			{
				BoostActive = true;
				BoostTimer = 0.0f;
				thurstfactor *= BoostThrustFactor;
				
			}

			if (BoostActive) {
				BoostTimer += frameTime; 
				// Check if boost is about to overheat
				if (BoostTimer >= BoostDuration - BoostOverheatWarningTime && !Boost_Over_Heat)
				{
					hover_car->SetSkin("Fire.jpg");
					myfont2->Draw("Warning: Boost Overheat in 1 second!", 750, 300, kRed, CentreAlign);
				
				}
				if (BoostTimer >= BoostDuration) {

					Boost_Over_Heat = true;
					BoostActive = false;
					hover_car->SetSkin("td_interstellar.jpg");
					thurstfactor /= BoostThrustFactor;
					cofficientofDrag *= 2.0f;
					BoostTimer = 0.0f; // Reset boost timer
				}
			}

			// Check for boost cooldown
			if (Boost_Over_Heat)
			{
				BoostTimer += frameTime;
				if (BoostTimer >= BoostCooldown)
				{
					Boost_Over_Heat = false;
					BoostTimer = 0.0f;
					cofficientofDrag /= 2.0f;
				}
			}


			//Camera Setup
			if (myEngine->KeyHeld(Key_Up))    camera->MoveLocalY(cameraspeed * frameTime);
			if (myEngine->KeyHeld(Key_Down))  camera->MoveLocalY(-cameraspeed * frameTime);
			if (myEngine->KeyHeld(Key_Right)) camera->MoveLocalX(cameraspeed * frameTime);
			if (myEngine->KeyHeld(Key_Left))  camera->MoveLocalX(-cameraspeed * frameTime);

			if (mouseCaptureActive && !chaseCamActive)
			{
				mouseMoveX += myEngine->GetMouseMovementX();
				mouseMoveY += myEngine->GetMouseMovementX();

				camera->ResetOrientation();
				camera->RotateLocalY(mouseMoveX * kMouseRotation);
				camera->RotateLocalX(mouseMoveY * kMouseRotation);

				float mouseMoveWheel = myEngine->GetMouseWheelMovement();
				camera->MoveLocalZ(mouseMoveWheel * kMouseWheelMove);
			}
			
			if (myEngine->KeyHit(Key_1))
			{

				mouseCaptureActive = !mouseCaptureActive;

				if (mouseCaptureActive)
				{
					myEngine->StartMouseCapture();
				}
				else
				{
					myEngine->StopMouseCapture();
				}
			}
			if (myEngine->KeyHit(Key_2))

			{
				chaseCamActive = !chaseCamActive;
				if (mouseCaptureActive)
				{
					camera->AttachToParent(hover_car);
					camera->ResetOrientation();
					mouseMoveX = 0.0f;
					mouseMoveY = 0.0f;
				}
				else
				{
					camera->DetachFromParent();
				}

			}
			
			// Stop if the Escape key is pressed
			if (myEngine->KeyHit(Key_Escape))
			{
				myEngine->Stop();
			}

		}
		
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

float distance(IModel* dummy, IModel* hover_car)
{
	float distX = hover_car->GetX() - dummy->GetX();
	float distZ = hover_car->GetZ() - dummy->GetZ();


	return sqrt((distX * distX) + (distZ * distZ));

}
float hover_car2checkpoint(IModel* dummy, IModel* hover_car, float hover_carmodel, float checkpointradius)
{
	float dist = distance(dummy, hover_car);
	return dist < (hover_carmodel + checkpointradius);
}

float hover_cartowall(IModel* hover_car, IModel* wall, float wall_length, float hover_carmodel, float wallDepth)
{
	float minX = wall->GetX() - (wall_length / 2) - 1 - hover_carmodel;
	float maxX = wall->GetX() + (wall_length / 2) - 1 - hover_carmodel;
	float minZ = wall->GetZ() - (wallDepth)-1 - hover_carmodel;
	float maxZ = wall->GetZ() + (wallDepth)-1 - hover_carmodel;


	return (hover_car->GetX() > minX && hover_car->GetX() < maxX && hover_car->GetZ() > minZ && hover_car->GetZ() < maxZ);

}
float hover_cartoisle(IModel* hover_car, IModel* isle_1, float isle_length, float hover_carmodel, float isleDepth)
{
	float minX = isle_1->GetX() - (isle_length / 2) - 1 - hover_carmodel;
	float maxX = isle_1->GetX() + (isle_length / 2) - 1 - hover_carmodel;
	float minZ = isle_1->GetZ() - (isleDepth)-1 - hover_carmodel;
	float maxZ = isle_1->GetZ() + (isleDepth)-1 - hover_carmodel;


	return (hover_car->GetX() > minX && hover_car->GetX() < maxX && hover_car->GetZ() > minZ && hover_car->GetZ() < maxZ);

}
float hover_cartotank(IModel* tank, IModel* hover_car, float tanklength, float tankRadius, float tankdepth)
{
	float min1X = tank->GetX() - (tanklength / 2) - tankRadius;
	float max1X = tank->GetX() + (tanklength / 2) - tankRadius;
	float min1Z = tank->GetZ() - (tankdepth / 2) - tankRadius;
	float max1Z = tank->GetZ() + (tankdepth / 2) - tankRadius;


	return (hover_car->GetX() > min1X && hover_car->GetX() < max1X && hover_car->GetZ() > min1Z && hover_car->GetZ() < max1Z);
}
float hover_cartotribune(IModel* tribune, IModel* hover_car, float tribunelength, float tribuneRadius, float tribunedepth)
{
	float min1Y = tribune->GetY() - (tribunelength / 2) - tribuneRadius;
	float max1Y = tribune->GetY() + (tribunelength / 2) - tribuneRadius;
	float min1Z = tribune->GetZ() - (tribunedepth / 2) - tribuneRadius;
	float max1Z = tribune->GetZ() + (tribunedepth / 2) - tribuneRadius;

	return (hover_car->GetY() > min1Y && hover_car->GetY() < max1Y && hover_car->GetZ() > min1Z && hover_car->GetZ() < max1Z);
}


float hover_cartonplayer(IModel* hover_car, IModel* nplayercar, float hover_carmodel, float hover_cardepth)
{
	float minX = nplayercar->GetX() - (hover_carmodel / 2) - 1 - hover_carmodel;
	float maxX = nplayercar->GetX() + (hover_carmodel / 2) - 1 - hover_carmodel;
	float minZ = nplayercar->GetZ() - (hover_cardepth)-1 - hover_carmodel;
	float maxZ = nplayercar->GetZ() + (hover_cardepth)-1 - hover_carmodel;


	return (hover_car->GetX() > minX && hover_car->GetX() < maxX && hover_car->GetZ() > minZ && hover_car->GetZ() < maxZ);

}

