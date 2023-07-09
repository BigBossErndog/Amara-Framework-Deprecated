namespace Amara {
    bool valueInRange(int val, int minVal, int maxVal) {
        return (val >= minVal) && (val <= maxVal);
    }

    bool valueInRange(float val, float minVal, float maxVal) {
        return (val >= minVal) && (val <= maxVal);
    }

    int max(int num1, int num2) {
        return (num1 > num2) ? num1 : num2;
    }
    float max(float num1, float num2) {
        return (num1 > num2) ? num1 : num2;
    }
    int min(int num1, int num2) {
        return (num1 < num2) ? num1 : num2;
    }
    float min(float num1, float num2) {
        return (num1 < num2) ? num1 : num2;
    }
}