#ifndef GF_WORKSPACE_R503S_H
#define GF_WORKSPACE_R503S_H


//#include <stdint.h>
#include <cstdint>

class R503S {
private:
    uint8_t address[4];
    uint8_t password[];

public:
    R503S() = default;

    ~R503S() = default;

    enum instruction_set {
        GetImage    = 0x01, //
        GenChar     = 0x02, //
        Match       = 0x03, //
        Search      = 0x04, //
        RegModel    = 0x05, //
        StoreChar   = 0x06, //
        LoadChar    = 0x07, //
        UpChar      = 0x08, //
        DownChar    = 0x09, //
        UpImage     = 0x0a, //
        DownImage   = 0x0b, //
        DeleteChar  = 0x0c, // from flash fingerprint library directly a model
        Empty       = 0x0d, // clear flash fingerprint library
        SetSysPara  = 0x0e, // set system parameters
        ReadSysPara = 0x0f, // read system parameters

        SetPwd      = 0x12, // set password
        VfyPwd      = 0x13, // verify password
        GetRandomCode = 0x14, // get random code
        SetAddr     = 0x15, // set address
        ReadInfPage = 0x16, // read information page

        WriteNotepad= 0x18, // write notepad
        ReadNotepad = 0x19, // read notepad

        ValidTemplateNum = 0x1d, // get valid template number

        ReadIndexTable = 0x1f, // read index table

        GetImageEx  = 0x28, // get image with quality check

        Cancel      = 0x30, // cancel current operation

        AuraLedConfig = 0x35, // configure aura led
        CheckSensor = 0x36, // check sensor status
        GetAlgVer   = 0x39, // get algorithm version
        GetFwVer    = 0x3a, // get firmware version
        ReadProInfo = 0x3c, // read product information
        SoftReset   = 0x3d, // soft reset

        HandShake   = 0x40, // handshake
    };

    enum error_code {
        NoError = 0x00, // no error
        ReceiveError = 0x01, // receive error
        SensorNotFinger = 0x02, // sensor is not finger
        RegFingerprintFailed = 0x03, // registration failed

        ImageTooMess = 0x06, // image is too mess
        ImageFeaturesTooFew = 0x07, // image features are too few
        FingerprintMismatch = 0x08, // fingerprint mismatch
        FingerprintNotFound = 0x09, // fingerprint not found
        FeatureMergingFailed = 0x0a, // feature merging failed
        IndexTableInvalid = 0x0b, // index table is invalid
        LoadModelFailed = 0x0c, // load model failed
        UpFeatureFailed = 0x0d, // up feature failed

    };
};


#endif //GF_WORKSPACE_R503S_H
