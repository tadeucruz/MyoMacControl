#include <stdio.h>
#include <myo/myo.hpp>

#define DIRETORIOBASE "/Users/tadeucruz/Documents/Xcode/MyoMacControl"

class DataCollector : public myo::DeviceListener {
    
    bool onArm;
    myo::Arm whichArm;
    
    bool isUnlocked;

    int roll_w, pitch_w, yaw_w;
    myo::Pose currentPose;
    
    public: DataCollector() : onArm(false), isUnlocked(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose()
    {
    }
    
    void onUnpair(myo::Myo* myo, uint64_t timestamp)
    {
        roll_w = 0;
        pitch_w = 0;
        yaw_w = 0;
        onArm = false;
        isUnlocked = false;
    }
    
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        using std::atan2;
        using std::asin;
        using std::sqrt;
        using std::max;
        using std::min;
        
        float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                           1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
        float pitch = asin(max(-1.0f, min(1.0f, 2.0f * (quat.w() * quat.y() - quat.z() * quat.x()))));
        float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                          1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
        
        roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
        pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
        yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
    }
    
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        currentPose = pose;
        
        if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
            myo->unlock(myo::Myo::unlockHold);
            myo->notifyUserAction();
        } else {
            myo->unlock(myo::Myo::unlockTimed);
        }
    }
    
    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState)
    {
        onArm = true;
        whichArm = arm;
    }
    
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
    {
        onArm = false;
    }
    
    void onUnlock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = true;
    }
    
    void onLock(myo::Myo* myo, uint64_t timestamp)
    {
        isUnlocked = false;
    }
    
    void process()
    {
        //Inicialmente utilizando execução de comandos.
        if (onArm) {

            std::string poseString = currentPose.toString();
            std::cout << poseString << std::endl;
            
            std::string comando = "osascript ";
            comando.append(DIRETORIOBASE);
            
            if ( "fist" == poseString ){
                comando.append("/AppleScripts/bloquearTela.SCPT");
                system(comando.c_str());
                return;
            }
            
            if ( "fingersSpread" == poseString ){
                comando.append("/AppleScripts/desbloquearTela.SCPT");
                system(comando.c_str());
                return;
            }
            
            if ( "waveIn" == poseString ){
                comando.append("/AppleScripts/volumaDescer.SCPT");
                system(comando.c_str());
                return;
            }
            
            if ( "waveOut" == poseString ){
                comando.append("/AppleScripts/volumeSubir.SCPT");
                system(comando.c_str());
                return;
            }

        }

    }
    
};