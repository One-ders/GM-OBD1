
extern struct Panel *RawData;
int init_RawData();
int update_RawData(int fd, char *indata);
int input_RawData(int fd, char *indata);

extern struct Panel *FlagData;
int init_FlagData();
int update_FlagData(int fd, struct ECM_Regs *eregs);
int input_FlagData(int fd, struct ECM_Regs *eregs);

extern struct Panel *SensorData;
int init_SensorData();
int update_SensorData(int fd, struct ECM_Regs *eregs);
int input_SensorData(int fd, struct ECM_Regs *eregs);

extern struct Panel *ErrorData;
int init_ErrorData();
int update_ErrorData(int fd, struct ECM_Regs *eregs);
int input_ErrorData(int fd, struct ECM_Regs *eregs);
