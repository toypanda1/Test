typedef struct KeyPair{
    unsigned int NCC;
    unsigned char NH[D_NSP_TDYTH_NAAAS_4G_NH_LEN];
};


typedef struct  St_NSP_TDYTH_NAAAS_4G_Header
{
    unsigned char ucVersion;     /*粗对结束后的仰角度，单位：弧度*/
    unsigned short usModuleCode;   /*粗对结束后的仰角度，单位：弧度*/
    unsigned char ucReqType;  /*粗对结束后的仰角度，单位：弧度*/
    unsigned char ucSubType;  /*粗对结束后的仰角度，单位：弧度*/
    unsigned char ucUserCategory;
    unsigned char ucStep;
};



typedef struct St_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucDstPort;
    unsigned int  aucSrcPort;
    unsigned int  iDataLen;
    unsigned char ucReqAuth;    //暂时定义0x01来定义用户身份请求标识
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
};




