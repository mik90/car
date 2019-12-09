// This is the interface between Arduino's motorController and Rpi's carController

// Max message size should be < 32
constexpr unsigned char maxMessageSize = 32;


unsigned int InvalidServoAngle = 9999;
unsigned int InvalidWheelSpeed = 9999;

enum MessageId_t {SERVO_CONTROL  = 0,
                 WHEEL_CONTROL   = 1,
                 ULTRASONIC_INFO = 2};
enum MotorDir_t  {FORWARD = 0,
                  REVERSE = 1,
                  RELEASE = 2};

// Pi -> Arduino --------------------
struct servoControl
{
    static constexpr unsigned char id = MessageId_t::SERVO_CONTROL;
    // Range is 0-100
    unsigned int panServoAngle  = InvalidServoAngle;
    unsigned int tiltServoAngle = InvalidServoAngle;
};
static_assert(sizeof(servoControl) <= maxMessageSize);

struct wheelControl
{
    static constexpr MessageId_t id = MessageId_t::WHEEL_CONTROL;
    // Range is 0-255
    unsigned int wheelSpeed = InvalidWheelSpeed;
    unsigned char leftSideDir  = MotorDir_t::RELEASE;
    unsigned char rightSideDir = MotorDir_t::RELEASE;
};
static_assert(sizeof(wheelControl) <= maxMessageSize);


// Arduino -> Pi --------------------
struct ultrasonicInfo
{
    static constexpr unsigned char id = MessageId_t::ULTRASONIC_INFO;
    unsigned int distance_cm = 0;
};
static_assert(sizeof(ultrasonicInfo) <= maxMessageSize);