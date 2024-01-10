import cv2

def capture_images():
    cap = cv2.VideoCapture(1)

    # Initialize image counter
    img_count = 0

    while True:
        ret, frame = cap.read()
        #frame = cv2.flip(frame,-1)
        cv2.imshow('Press Space to Capture (Esc to Exit)', frame)

        # Check for the spacebar key press
        if cv2.waitKey(1) == 32:  # 32 is the ASCII code for the spacebar
            img_name = f"Test{img_count}.png"
            cv2.imwrite(img_name, frame)
            print(f"Image {img_name} captured and saved.")
            img_count += 1

        # Check for the Esc key press to exit the loop
        if cv2.waitKey(1) == 27:
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    capture_images()
