#include <iostream>
#include <time.h>
#include <sstream>

#include "game.h"
#include "path_config.h"
#include <SOIL\SOIL.h>


namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Main window settings
const std::string window_title_g = "Demo";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const bool window_full_screen_g = false;

// Viewport and camera settings
float camera_near_clip_distance_g = 0.01;
float camera_far_clip_distance_g = 1000.0;
float camera_fov_g = 20.0; // Field-of-view of camera
const glm::vec3 viewport_background_color_g(0.0, 0.0, 0.0);
glm::vec3 camera_position_g(0.5, 0.5, 10.0);
glm::vec3 camera_look_at_g(0.0, 0.0, 0.0);
glm::vec3 camera_up_g(0.0, 1.0, 0.0);

// Materials 
const std::string material_directory_g = MATERIAL_DIRECTORY;

// Global texture info


Game::Game(void){

    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void){

    // Run all initialization steps
    InitWindow();
    InitView();
    InitEventHandlers();

    // Set variables
    animating_ = true;
}

       
void Game::InitWindow(void){

    // Initialize the window management library (GLFW)
    if (!glfwInit()){
        throw(GameException(std::string("Could not initialize the GLFW library")));
    }

    // Create a window and its OpenGL context
    if (window_full_screen_g){
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), glfwGetPrimaryMonitor(), NULL);
    } else {
        window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g.c_str(), NULL, NULL);
    }
    if (!window_){
        glfwTerminate();
        throw(GameException(std::string("Could not create window")));
    }

    // Make the window's context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK){
        throw(GameException(std::string("Could not initialize the GLEW library: ")+std::string((const char *) glewGetErrorString(err))));
    }
}


void Game::InitView(void){

    // Set up z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Set viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    // Set up camera
    // Set current view
    camera_.SetView(camera_position_g, camera_look_at_g, camera_up_g);
    // Set projection
    camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


void Game::InitEventHandlers(void){

    // Set event callbacks
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set pointer to game object, so that callbacks can access it
    glfwSetWindowUserPointer(window_, (void *) this);
}


void Game::SetupResources(void){

    // Create geometry of the "wall"
	resman_.CreateWall("WallMesh");
	resman_.CreateTorus("TorusMesh");
    resman_.CreateTorus("RacingPathWayMesh",10.0f,1.0f);
    resman_.CreateSphere("SphereMesh", 0.3f,10.0f, 10.0f);

    // Load material to be used for normal mapping
    std::string filename = std::string(MATERIAL_DIRECTORY) + std::string("/normal_map");
    resman_.LoadResource(Material, "NormalMapMaterial", filename.c_str());

    // Load texture to be used in normal mapping
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/normal_map2.png");
    resman_.LoadResource(Texture, "NormalMap", filename.c_str());

    //Load object meshes

    filename = std::string(MATERIAL_DIRECTORY) + std::string("/cow.obj");
    resman_.LoadResource(Mesh, "CowMesh", filename.c_str());


    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Spaceship.obj");
    resman_.LoadResource(Mesh, "SpaceshipMesh", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("/Sniper_Rifle.obj");
    resman_.LoadResource(Mesh, "Sniper_Rifle", filename.c_str());

    // Load texture to be used on the object
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/rocky.png");
    resman_.LoadResource(Texture, "RockyTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("/blueBackground.jpg");
    resman_.LoadResource(Texture, "BlueBackgroundTexture", filename.c_str());

    filename = std::string(MATERIAL_DIRECTORY) + std::string("/spaceBackground.png");
    resman_.LoadResource(Texture, "SpaceBackgroundTexture", filename.c_str());

    // shader for 3-term lighting effect
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/lit");
    resman_.LoadResource(Material, "Lighting", filename.c_str());


    // Load material to be applied to particles
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/fire");
    resman_.LoadResource(Material, "FireMaterial", filename.c_str());

    // Create particles for explosion
    resman_.CreateTorusParticles("TorusParticles");
    resman_.CreateSphereParticles("SphereParticles");

    // Load texture to be applied to particles
    filename = std::string(MATERIAL_DIRECTORY) + std::string("/flame4x4orig.png");
    resman_.LoadResource(Texture, "Flame", filename.c_str());

}


void Game::SetupScene(void){

    // Set background color for the scene
    scene_.SetBackgroundColor(viewport_background_color_g);

    // Create an instance of the wall
	//game::SceneNode *wall = CreateInstance("WallInstance1", "WallMesh", "NormalMapMaterial", "NormalMap");
	//game::SceneNode *torus = CreateInstance("WallInstance2", "TorusMesh", "NormalMapMaterial", "NormalMap");
    SceneNode* playerSpaceShip = CreateInstance(PlayerObject, "PlayerSpaceship", "SpaceshipMesh", "Lighting", "RockyTexture");
    SceneNode* weapon_1 = CreateInstanceWithoutAddingToGraph(WeaponObject, "Weapon", "Sniper_Rifle", "Lighting", "BlueBackgroundTexture");
    SceneNode* weapon_2 = CreateInstanceWithoutAddingToGraph(WeaponObject, "Weapon", "Sniper_Rifle", "Lighting", "BlueBackgroundTexture");
    weapon_1->SetPosition(glm::vec3(0.0f,0.0f,0.5f));
    weapon_2->SetPosition(glm::vec3(0.0f, 0.0f, -0.5f));

    weapon_1->SetOrientation(glm::normalize(glm::angleAxis(90.0f * glm::pi<float>() / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f))));
    weapon_2->SetOrientation(glm::normalize(glm::angleAxis(90.0f * glm::pi<float>() / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f))));
    playerSpaceShip->addAChild(weapon_1);
    playerSpaceShip->addAChild(weapon_2);

    //SceneNode* playerSpaceShip = CreateInstance(PlayerObject, "PlayerSpaceship", "SphereMesh", "NormalMapMaterial", "NormalMap");
    //glm::quat ori = glm::angleAxis(glm::radians(-180.0f), glm::normalize(glm::vec3(0, 1, 0)));
    //playerSpaceShip->SetOrientation(ori);
    scene_.playerControlledObject = playerSpaceShip;
    camera_.SetParentRotationAngle(playerSpaceShip->GetDirecationAngularResult());
    camera_.SetParent_position(playerSpaceShip->GetPosition());
   //CreateAsteroidField(300);
    CreateRunningPath();

    //SceneNode* cow = CreateInstance(SceneNodeObject,"TestingObject", "CowMesh", "Lighting", "RockyTexture");

}

void Game::MainLoop(void){

    // Loop while the user did not close the window
    while (!glfwWindowShouldClose(window_)){
        // Animate the scene
        if (animating_){
            static double last_time = 0;
            double current_time = glfwGetTime();
            if ((current_time - last_time) > 0.01){
                
            }
            float deltaTime = current_time - last_time;
            scene_.Update(deltaTime);

            if (scene_.playerControlledObject != nullptr) {
                Player* player = dynamic_cast<Player*>(scene_.playerControlledObject);
                playerKeyCallBack(player);
                camera_.SetPosition(glm::vec3(0,0,-10.0f));
                camera_.SetParent_position(player->GetPosition());
                camera_.SetParentRotationAngle(player->GetDirecationAngularResult());
                camera_.SetParentRotation(player->GetOrientation());
                camera_.SetParentVelocity(player->GetVelocity());
                //camera_.SetOrientation(player->cameraFollowingRotation());
                camera_.Roll(player->GetBalanceAngle());
            
            }

            //update the time 
            last_time = current_time;
        }

        // Draw the scene
        scene_.Draw(&camera_);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}


void Game::playerKeyCallBack(Player* playerObj) {
    // Get user data with a pointer to the game class
    void* ptr = glfwGetWindowUserPointer(window_);
    Game* game = (Game*)ptr;

    //reset player acceleration
    playerObj->SetAcceleration(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::quat ori = glm::angleAxis(glm::radians(-0.00f), glm::normalize(glm::vec3(1, 1, 1)));
    playerObj->SetOrientation_acceleration_(glm::vec3(0,0,0));


    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        playerObj->SetAcceleration(glm::vec3(0.0f, 0.0f, -24.0f));
    }
    if (glfwGetKey(window_, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        playerObj->SetAcceleration(glm::vec3(0.0f, 0.0f,14.0f));
    }
    if (glfwGetKey(window_, GLFW_KEY_LEFT) == GLFW_PRESS) {
        glm::quat ori = glm::angleAxis(glm::radians(0.9f), glm::normalize(glm::vec3(0, 1, 0)));

        playerObj->SetOrientation_acceleration_(glm::vec3(0,0.9f,0));

        //adding visual effect- tilt
        float balanceAngle = playerObj->GetBalanceAngle();
        //playerObj->SetBalanceAngle(balanceAngle - 0.003f);
    }
    if (glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        glm::quat ori = glm::angleAxis(glm::radians(-0.9f), glm::normalize(glm::vec3(0, 1, 0)));
        playerObj->SetOrientation_acceleration_(glm::vec3(0, -0.9f, 0));


        //adding visual effect - tilt
        float balanceAngle = playerObj->GetBalanceAngle();
        //playerObj->SetBalanceAngle(balanceAngle + 0.003f);
    }

    if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS) {
        glm::quat ori = glm::angleAxis(glm::radians(-1.9f), glm::normalize(glm::vec3(1, 0, 0)));

        playerObj->SetOrientation_acceleration_(glm::vec3(-1.9f, 0, 0));
    }

    if (glfwGetKey(window_, GLFW_KEY_UP) == GLFW_PRESS) {
        glm::quat ori = glm::angleAxis(glm::radians(1.9f), glm::normalize(glm::vec3(1, 0, 0)));

        playerObj->SetOrientation_acceleration_(glm::vec3(1.9f, 0, 0));
    }
    
    if (glfwGetKey(window_, GLFW_KEY_F) == GLFW_PRESS) {

        vector<SceneNode*> children = playerObj->GetChildren();
        for (int count = 0; count < children.size(); count++) {
            if (children[count]->GetName().find("Weapon") != std::string::npos) {
                //ignore the collision
                Gun* weapon = dynamic_cast<Gun*>(children[count]);
                if (weapon->Fire()) {
                    SceneNode* bullet = CreateInstanceWithoutAddingToGraph(BulletObject, "Bullet", "SphereParticles", "FireMaterial", "Flame");
                    bullet->SetBlending(true);
                    glm::vec3 speed = playerObj->GetOrientation() * glm::vec3(0.0f, 0.0f, -10.0f) * glm::inverse(playerObj->GetOrientation());

                    //cout << "Speed " << speed.x << speed.y << speed.z << endl;
                    bullet->SetPosition(playerObj->GetPosition());
                    bullet->SetAcceleration(speed);
                    bullet->SetOrientation(playerObj->GetOrientation());
                    scene_.AddNode(bullet);
                
                }
                

            }
            
        }
        
    }

}


void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

    // Get user data with a pointer to the game class
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;

    // Quit game if 'q' is pressed
    if (key == GLFW_KEY_Q && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    

    // View control
    /*
    * 
    * // Stop animation if space bar is pressed
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        game->animating_ = (game->animating_ == true) ? false : true;
    }

    float rot_factor(glm::pi<float>() / 180);
    float trans_factor = 1.0;
    if (key == GLFW_KEY_UP){
        game->camera_.Pitch(rot_factor);
    }
    if (key == GLFW_KEY_DOWN){
        game->camera_.Pitch(-rot_factor);
    }
    if (key == GLFW_KEY_LEFT){
        game->camera_.Yaw(rot_factor);
    }
    if (key == GLFW_KEY_RIGHT){
        game->camera_.Yaw(-rot_factor);
    }
    if (key == GLFW_KEY_S){
        game->camera_.Roll(-rot_factor);
    }
    if (key == GLFW_KEY_X){
        game->camera_.Roll(rot_factor);
    }
    if (key == GLFW_KEY_A){
        game->camera_.Translate(game->camera_.GetForward()*trans_factor);
    }
    if (key == GLFW_KEY_Z){
        game->camera_.Translate(-game->camera_.GetForward()*trans_factor);
    }
    if (key == GLFW_KEY_J){
        game->camera_.Translate(-game->camera_.GetSide()*trans_factor);
    }
    if (key == GLFW_KEY_L){
        game->camera_.Translate(game->camera_.GetSide()*trans_factor);
    }
    if (key == GLFW_KEY_I){
        game->camera_.Translate(game->camera_.GetUp()*trans_factor);
    }
    if (key == GLFW_KEY_K){
        game->camera_.Translate(-game->camera_.GetUp()*trans_factor);
    }
    */
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height){

    // Set up viewport and camera projection based on new window size
    glViewport(0, 0, width, height);
    void* ptr = glfwGetWindowUserPointer(window);
    Game *game = (Game *) ptr;
    game->camera_.SetProjection(camera_fov_g, camera_near_clip_distance_g, camera_far_clip_distance_g, width, height);
}


Game::~Game(){
    
    glfwTerminate();
}


Asteroid *Game::CreateAsteroidInstance(std::string entity_name, std::string object_name, std::string material_name){

    // Get resources
    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    // Create asteroid instance
    Asteroid *ast = new Asteroid(entity_name, geom, mat);
    scene_.AddNode(ast);
    return ast;
}


void Game::CreateAsteroidField(int num_asteroids){

    // Create a number of asteroid instances
    for (int i = 0; i < num_asteroids; i++){
        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "AsteroidInstance" + index;

        std::string object_name = "TorusMesh";
        std::string material_name = "NormalMapMaterial";
        std::string texture_name = "NormalMap";

        Resource* geom = resman_.GetResource(object_name);
        if (!geom) {
            throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
        }

        Resource* mat = resman_.GetResource(material_name);
        if (!mat) {
            throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
        }

        Resource* tex = NULL;
        if (texture_name != "") {
            tex = resman_.GetResource(texture_name);
            if (!tex) {
                throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
            }
        }

        // Create asteroid instance
        SceneNode* ast = new SceneNode(name, geom, mat, tex);


        // Set attributes of asteroid: random position, orientation, and
        // angular momentum

        ast->SetPosition(glm::vec3(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5));
        //ast->SetPosition(glm::vec3(-1.0 + 2.0*((float) rand() / RAND_MAX), -10.0 + 20.0*((float) rand() / RAND_MAX), 20.0*((float) rand() / RAND_MAX)));
       
        //ast->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
    }
}

void Game::CreateRunningPath() {
    vector<glm::vec3> input;
    vector<glm::vec3> output;
    
    input.push_back(glm::vec3(0, 0, -1));
    input.push_back(glm::vec3(0, 50, -200));
    input.push_back(glm::vec3(50, 70, -300));
    input.push_back(glm::vec3(0, 50, -510));
    input.push_back(glm::vec3(50, 50, -710));
    input.push_back(glm::vec3(100, 50, -910));



    glm::vec3 direcationOfTour;
    glm::vec3 perviousTourPosition = input[0];
    float angleOfPath = 0.0f;
    PathCalculationProcessor path;
    output = path.calculateThePath(input);

    //cout << output.size() << endl;
    for (int i = 0; i < output.size(); i++) {
        // Create instance name
        std::stringstream ss;
        ss << i;
        std::string index = ss.str();
        std::string name = "AsteroidInstance" + index;
        std::string blockerName = "BlockerInstance" + index;
        std::string object_name = "RacingPathWayMesh";
        std::string material_name = "Lighting";
        std::string texture_name = "SpaceBackgroundTexture";

        Resource* geom = resman_.GetResource(object_name);
        if (!geom) {
            throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
        }

        Resource* mat = resman_.GetResource(material_name);
        if (!mat) {
            throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
        }

        Resource* tex = NULL;
        if (texture_name != "") {
            tex = resman_.GetResource(texture_name);
            if (!tex) {
                throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
            }
        }

        // Create asteroid instance
        RacingPathObject* ast = new RacingPathObject(blockerName, geom, mat, tex);
       

        // Set attributes of asteroid: random position, orientation, and
        // angular momentum

        //cout << i << " is position (" << output[i].x << " , " << output[i].y << " , " << output[i].z << endl;

        float currentAngle = glm::acos((glm::dot(output[i], perviousTourPosition)) / (glm::length(output[i]) * glm::length(perviousTourPosition)));
        angleOfPath += currentAngle;
        //cout << currentAngle << endl;
        ast->SetPosition(output[i]);
        ast->SetOrientation(glm::normalize(glm::angleAxis(angleOfPath* glm::pi<float>()/180.0f, glm::vec3(1.0f, 0.0f, 0.0f))));
        perviousTourPosition = output[i];

        //* 180.0f / glm::pi<float>()

        //ranomly give a speed boost
        if (rand() % 3 + 1 == 2) {
            ast->SetAccelrationCircle(true);
        }

        scene_.AddNode(ast);

        if (rand() % 5 + 1 == 2) {
            Blocker* blocker = new Blocker(name, resman_.GetResource("SphereMesh"), mat, tex);
            blocker->SetPosition(output[i] + glm::vec3(rand() % 10 - 5.2f));
            scene_.AddNode(blocker);
        }
        
        //ast->SetAngM(glm::normalize(glm::angleAxis(0.05f*glm::pi<float>()*((float) rand() / RAND_MAX), glm::vec3(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX)))));
    }

}

SceneNode* Game::CreateInstance(int ObjectType, std::string entity_name, std::string object_name, std::string material_name, std::string texture_name){

    Resource *geom = resman_.GetResource(object_name);
    if (!geom){
        throw(GameException(std::string("Could not find resource \"")+object_name+std::string("\"")));
    }

    Resource *mat = resman_.GetResource(material_name);
    if (!mat){
        throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
    }

    Resource *tex = NULL;
    if (texture_name != ""){
        tex = resman_.GetResource(texture_name);
        if (!tex){
            throw(GameException(std::string("Could not find resource \"")+material_name+std::string("\"")));
        }
    }

    SceneNode *scn = scene_.CreateNode(true,ObjectType,entity_name, geom, mat, tex);
    return scn;
}

SceneNode* Game::CreateInstanceWithoutAddingToGraph(int ObjectType, std::string entity_name, std::string object_name, std::string material_name, std::string texture_name) {

    Resource* geom = resman_.GetResource(object_name);
    if (!geom) {
        throw(GameException(std::string("Could not find resource \"") + object_name + std::string("\"")));
    }

    Resource* mat = resman_.GetResource(material_name);
    if (!mat) {
        throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
    }

    Resource* tex = NULL;
    if (texture_name != "") {
        tex = resman_.GetResource(texture_name);
        if (!tex) {
            throw(GameException(std::string("Could not find resource \"") + material_name + std::string("\"")));
        }
    }

    SceneNode* scn = scene_.CreateNode(false, ObjectType, entity_name, geom, mat, tex);
    return scn;
}

} // namespace game
