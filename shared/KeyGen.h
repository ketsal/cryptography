class KeyGen
{
private:
public:
    KeyGen()
    {
    }
    unsigned char *GenerateTripleDesKey();
    unsigned char *GenerateDesKey();
    unsigned char *GenerateAesKey(int size);
    unsigned char *RC4(int size);
    unsigned char *Otp(int size);
	void GenerateRSAkeys(std::string PathToPublic,std::string PathToPrivate,int bits);
};