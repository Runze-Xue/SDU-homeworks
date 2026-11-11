#include<string.h>
#include<tchar.h>
using namespace std;
class CLIENT {
public:
	CLIENT();
	static void ChangeServerName(const char change[100]);
	static void ShowServeName();
	static void getClientNumber();
private:
	static char ServerName[100];
	static int ClientNum;
};
