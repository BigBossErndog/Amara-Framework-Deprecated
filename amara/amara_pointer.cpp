namespace Amara {
    class Pointer: public FloatVector2 {
        public:
            bool isDown = false;
            bool justDown = false;

            bool isActivated = false;

			int recX = 0;
			int recY = 0;
			int movementX = 0;
			int movementY = 0;

            virtual void manage() {
                isActivated = false;

				movementX = x - recX;
				movementY = y - recY;
				recX = x;
				recY = y;
            }
    };
}