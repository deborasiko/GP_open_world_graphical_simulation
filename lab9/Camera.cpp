#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        //TODO
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraFrontDirection = glm::normalize(cameraPosition - cameraTarget);
        this->cameraRightDirection = glm::cross(cameraFrontDirection, cameraUp);
        this->cameraUpDirection = cameraUp;
        this->yaw = -90.0f; // Initial yaw
        this->pitch = 0.0f; // Initial pitch

    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        //TODO

        return glm::lookAt(cameraPosition, cameraTarget, this->cameraUpDirection);
    }

    glm::vec3 Camera::getCameraTarget()
    {
        return cameraTarget;
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO
        glm::vec3 moveDirection;

        if (direction == MOVE_FORWARD)
            moveDirection = glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
        if (direction == MOVE_BACKWARD)
            moveDirection = glm::vec3(-cameraFront.x, 0.0f, -cameraFront.z);
        if (direction == MOVE_LEFT)
            moveDirection = -glm::normalize(glm::cross(cameraFront, cameraUpDirection)) * glm::vec3(1.0f, 0.0f, 1.0f);
        if (direction == MOVE_RIGHT)
            moveDirection = glm::normalize(glm::cross(cameraFront, cameraUpDirection)) * glm::vec3(1.0f, 0.0f, 1.0f);

        cameraPosition += speed * moveDirection;

        // Ensure the camera stays at the fixed height
        cameraPosition.y = 1.0f;

        // Update cameraTarget based on new position
        this->cameraTarget = this->cameraPosition + this->cameraFront;
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO
        this->yaw += yaw;
        this->pitch += pitch;

        // Constrain the pitch
        if (this->pitch > 89.0f)
            this->pitch = 89.0f;
        if (this->pitch < -89.0f)
            this->pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->cameraFront = glm::normalize(front);

        // Update cameraTarget based on new direction
        this->cameraTarget = this->cameraPosition + this->cameraFront;
    }

    void Camera::startAnimation() {
        isAnimating = true;
        animationProgress = 0.0f;
        startPosition = cameraPosition;
        targetPosition = glm::vec3(0.0f, 2.0f, 1.0f);
    }

    void Camera::updateAnimation() {
        if (!isAnimating) return;

        animationProgress += animationSpeed;
        if (animationProgress >= 1.0f) {
            animationProgress = 1.0f;
            isAnimating = false;
        }//nem biztos tho, mert a rendering viselkedik furan, hat nem ad neked kepet kozben

        // Interpolate position
        cameraPosition = glm::mix(startPosition, targetPosition, animationProgress);

        // Update cameraTarget if needed
        this->cameraTarget = this->cameraPosition + this->cameraFrontDirection;//nobody: nothing nanosuit: wa
        //nem engedte bezarni:)) try it
    }
}
