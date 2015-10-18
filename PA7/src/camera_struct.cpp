#ifndef __CAMERA_STRUCT_CPP_
#define __CAMERA_STRUCT_CPP_

//Camera position variables
glm::vec3 CameraPosition( 0.0, 8.0, -8.0 );
glm::vec3 CameraFocus( 0.0, 0.0, 0.0 );
glm::vec3 CameraYaw( 0.0, 1.0, 0.0 );
glm::vec3 CameraDirection;
glm::vec3 Camera_UP;
glm::vec3 Camera_RIGHT;
struct {
    glm::vec3 startingCameraPos;
    glm::vec3 endingCameraPos;

    glm::vec3 startingCameraFocus;
    glm::vec3 endingCameraFocus;

    int frame = MAX_FRAME;
    int planetToTrack = -1;
    struct {
        glm::vec3 midCameraPos;
        glm::vec3 midCameraFocus;
    } keyframes [MAX_FRAME];

    void setAllFrames() {
        for( int i = 0; i < MAX_FRAME; i++ ){
            float ratio = float(i)/float(MAX_FRAME);
            //fill in the intermediate keyframes
            keyframes[i].midCameraPos = ((1-ratio) * startingCameraPos) + (ratio * endingCameraPos);
            keyframes[i].midCameraFocus = ((1-ratio) * startingCameraFocus) + (ratio * endingCameraFocus);
        }
        frame = 0;
        planetToTrack = -1;
    }

    void updateCamera(){
        if( planetToTrack >= 0 ){
            //get that model's thing and update all the time
            CameraFocus = glm::vec3( solarsystem[indexOfSystem].getPlanetPointer(planetToTrack)->getModel() * glm::vec4(0.0, 0.0, 0.0, 1.0));
            CameraPosition = CameraFocus + glm::vec3( 0.0, .5, -.5 );
        }
        else if( frame < MAX_FRAME ){
            CameraPosition = keyframes[frame].midCameraPos;
            CameraFocus = keyframes[frame].midCameraFocus;
            frame++;
        }
        view = glm::lookAt( CameraPosition, //Eye Position
                    CameraFocus, //Focus point
                    CameraYaw ); //Positive Y is up

        CameraDirection = glm::normalize( CameraPosition - CameraFocus );
        Camera_RIGHT = glm::normalize( glm::cross(CameraYaw, CameraDirection) );
        Camera_UP = glm::normalize( glm::cross(CameraDirection, Camera_RIGHT) );
    }

} CameraAnimation;


#endif