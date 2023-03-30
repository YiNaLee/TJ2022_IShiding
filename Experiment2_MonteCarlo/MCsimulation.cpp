#include "MonteCarlo_simulation.h"

double MC::ierfc(const double y)
// inverse of the error function erfc
// Copyright(C) 1996 Takuya OOURA (email: ooura@mmm.t.u-tokyo.ac.jp)
{
    const double IERFC_LIM = 27;
    double z = (y > 1) ? 2 - y : y;
    if (z < 1e-300) return (y > 1) ? -IERFC_LIM : IERFC_LIM;
    double w = 0.916461398268964 - log(z);
    double u = sqrt(w);
    double s = (log(u) + 0.488826640273108) / w;
    double t = 1 / (u + 0.231729200323405);
    double x = u * (1 - s * (s * 0.124610454613712 + 0.5)) -
        ((((-0.0728846765585675 * t + 0.269999308670029) * t +
            0.150689047360223) * t + 0.116065025341614) * t +
            0.499999303439796) * t;
    t = 3.97886080735226 / (x + 3.97886080735226);
    u = t - 0.5;
    s = (((((((((0.00112648096188977922 * u +
        1.05739299623423047e-4) * u - 0.00351287146129100025) * u -
        7.71708358954120939e-4) * u + 0.00685649426074558612) * u +
        0.00339721910367775861) * u - 0.011274916933250487) * u -
        0.0118598117047771104) * u + 0.0142961988697898018) * u +
        0.0346494207789099922) * u + 0.00220995927012179067;
    s = ((((((((((((s * u - 0.0743424357241784861) * u -
        0.105872177941595488) * u + 0.0147297938331485121) * u +
        0.316847638520135944) * u + 0.713657635868730364) * u +
        1.05375024970847138) * u + 1.21448730779995237) * u +
        1.16374581931560831) * u + 0.956464974744799006) * u +
        0.686265948274097816) * u + 0.434397492331430115) * u +
        0.244044510593190935) * t -
        z * exp(x * x - 0.120782237635245222);
    x += s * (x * s + 1);
    return (y > 1) ? -x : x;
}
double MC::inverse_Q(const double y)
{
    return ierfc(y * 2.0) * sqrt(2);
}
//Q�������ɱ�׼erfc�õ�
//������ѧ�Ƶ���Q(x)=erfc(x/��2)/2.0
double MC::Q(const double y)
{
    return erfc(y / sqrt(2)) / 2.0;

}
//��ʼ�����캯����
//����W���У�
//�ֻ�����
/*void swap(int* a, int* b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int randint(int a, int b) {
    return rand() % (b - a + 1) + a;
}

void randomize(int arr[N], int len) {
    int i;
    int j;
    for (i = 0; i < len; i++) {
        j = randint(i, len - 1);
        swap(&arr[i], &arr[j]);
    }
}*/
MC::MC()
{
    //�������50����
    pfa_theoretical[0] = 1e-6;
    pfa_theoretical[1] = 1e-5;
    for (int i = 1; i < 10; i++) {
        pfa_theoretical[i] = 1e-5 * i;
    }//1*1e-5 ~9*1e-5
    pfa_theoretical[10] = 1e-4;
    for (int i = 11; i < 19; i++) {
        pfa_theoretical[i] = 1e-4 * (i - 9);
    }//1e-4 ~ 9*1e-4
    pfa_theoretical[19] = 1e-3;
    for (int i = 20; i < 28; i++) {
        pfa_theoretical[i] = 1e-3 * (i - 18);
    }//1e-3 ~9*1e-3
    pfa_theoretical[28] = 1e-2;
    for (int i = 29; i < 37; i++) {
        pfa_theoretical[i] = 1e-2 * (i - 27);
    }//1e-2~ 9*1e-2

    pfa_theoretical[37] = 1e-1;
    pfa_theoretical[38] = 0.2;
    pfa_theoretical[39] = 0.3;
    pfa_theoretical[40] = 0.4;
    pfa_theoretical[41] = 0.5;
    pfa_theoretical[42] = 0.6;
    //pfa_theoretical[6] = 1;
    W = new int[N];
    u_S = new double[N];
    inset_S = new double[N];
    X = new double[N];
    if (W==NULL||u_S==NULL||inset_S==NULL||X==NULL) {
        cout << "��̬�ڴ�����ʧ�ܣ�\n";
        exit(-1);
    }
    for (int i = 0; i < N; i++) {
        if (i % 2 == 0){
            W[i] = 1;
        }
        else {
            W[i] = -1;
        }
    }
    
}
MC::~MC()
{
    delete[]W;
}
//�������ܣ���������ֲ�
int gen_data::Bernoulli_distribution()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(0.5);
    int a = d(gen);
    return a;
}
double MC::cal_phi(const double pfa)
{//��������ֵ ���ݹ�ʽ��=[��/sqrt(N)]*Q-1pfa
    return (STD / sqrt(N)) * ierfc(2*pfa) * sqrt(2);
}//�����ʽ����һ��Ӧ���ǶԵġ�������

//������֪��pfa����pm
double MC::cal_pmtheory(const double pfa,const double a)
{//��ʽ��ppt SS-ch-doc.pdf p8
    //pm=1-Q[Q-1(pfa)-sqrt(N)*a/STDx]
    return 1 - Q(inverse_Q(pfa) - sqrt(N) * a / STD);

}
double MC::cal_attack_pmtheory(const double pfa, const double a, const double STDV)
{//��ʽ������SS-ch-doc.pdf p32
    //pm=1-Q[Q-1(pfa)-sqrt(N)*a/sqrt(STDx^2+STDv^2)]
    return 1 - Q(inverse_Q(pfa) - sqrt(N) * a / sqrt(STD*STD+STDV*STDV));
}
//�������ܣ�����1000��c=0.5��GGD�ֲ������
//c=0.5ʱ����������������ָ���ֲ����
void gen_data::gen_GGDhalfone()
{
    random_device rd;
    mt19937_64 gen(rd());
    exponential_distribution<double> dis(1.0);
    double* data2 = new double[N];
    for (int i = 0; i < N; i++) {
        data[i] = dis(gen);
       
    }
    srand((unsigned)time(NULL));
    for (int i = 0; i < N; i++) {
        data2[i] = dis(gen);
        data[i] += data2[i];
       
        data[i] = data[i] * data[i];
        //���ȡ1��һ�����ݶ���ֲ������
        data[i] = rand() % 2 ? data[i] : -data[i];//�ӿ��ٶ�
    }
    
}
//�������ܣ�����C=1.0��GGD�ֲ�
//C=1.0ʱ��GGD����ָ���ֲ��ı�ʽ
void gen_data::gen_GGDone()
{
    random_device rd;
    mt19937_64 gen(rd());
    exponential_distribution<double> dis(1.0);
    
    for (int i = 0; i < N; i++) {
        data[i] = dis(gen);
        srand((unsigned)time(0));
        data[i] = rand() % 2 ? data[i] : -data[i];//�ӿ��ٶ�
    }
    
}
//�������ܣ�����c=2.0��GGD�ֲ�
//c=2.0ʱ��GGD���Ǹ�˹�ֲ�
void gen_data::gen_GGDtwo()
{
    random_device rd;
    mt19937_64 gen(rd());
    normal_distribution<double> dis(0,10.0);
    for (int i = 0; i < N; i++) {
        data[i] = dis(gen);
    }
}

gen_data::gen_data()
{
    data = new double[N];

}
gen_data::~gen_data()
{
    delete[]data;
}

void MC::test_no_attack(double a)
{
    for (int j = 0; j < DECT; j++) {
        gen_data G;
        G.gen_GGDtwo();//c=2.0
        //G.gen_GGDone();//c=1.0
        //G.gen_GGDhalfone();//c=0.5
        memcpy(X, G.data, sizeof(double) * N);
        memcpy(u_S, X, sizeof(double) * N);
        //Ƕ����̣�
        for (int i = 0; i < N; i++) {
            if (W[i] < 0) {
                inset_S[i] = X[i] - a;
            }
            else {
                inset_S[i] = X[i] + a;
            }
            //inset_S[i] = X[i] + a * W[i];
        }//Ƕ��
        double cor = correlator(u_S);
        double cor2= correlator(inset_S);
        for (int i = 0; i < BIN; i++) {
            if (cor > phi[i]) {
                cnterror[i]++;//������
            }
            if (cor2 < phi[i]) {
                cntmiss[i]++;//©�����
            }
        }
    }
    cout << "debug:ʵ����������\n";
    for (int i = 0; i < BIN; i++) {
        cout << cnterror[i] << endl;
    }
    cout << "debug:ʵ��©�������\n";
    for (int i = 0; i < BIN; i++) {
        cout << cntmiss[i] << endl;
    }
    for (int i = 0; i < BIN; i++) {
        pfa_experimental[i] = cnterror[i]*1.0 / DECT;
        pm_experimental[i] = cntmiss[i] * 1.0 / DECT;
    }
    cout << "debug:ʵ������ʣ�\n";
    for (int i = 0; i < BIN; i++) {
        cout << pfa_experimental[i] << endl;
    }
    cout << "debug:ʵ��©���ʣ�\n";
    for (int i = 0; i < BIN; i++) {
        cout << pm_experimental[i] << endl;
    } 

}
double MC::correlator(double *S)
{
    double L = 0;
    int cnt = 0;
    for (int i = 0; i < N; i++) {
        if (W[i] < 0) {
            L -= S[i];
        }
        else {
            L += S[i];
        }

    }
    L /= N;
   
    return L;
   
}


void MC::test_attack(double a,double STDV)
{
    gen_data Voice;
    for (int j = 0; j < DECT; j++) {
        gen_data G;
        random_device rd;
        mt19937_64 gen(rd());
        normal_distribution<double> dis(0, 10.0);
        normal_distribution<double> disVoice(0, STDV);//��˹��������
        for (int i = 0; i < N; i++) {
            X[i] = dis(gen);
            Voice.data[i] = disVoice(gen);
            X[i] += Voice.data[i];
        }//������˹�ֲ������,��������V
        memcpy(u_S, X, sizeof(double) * N);
        //Ƕ����̣�
        for (int i = 0; i < N; i++) {
            if (W[i] < 0) {
                inset_S[i] = X[i] - a;
            }
            else {
                inset_S[i] = X[i] + a;
            }
            //inset_S[i] = X[i] + a * W[i];
        }//end ofǶ��
        double cor = correlator(u_S);
        double cor2 = correlator(inset_S);
        for (int i = 0; i < BIN; i++) {
            if (cor > phi[i]) {
                cnterror[i]++;//������
            }
            if (cor2 < phi[i]) {
                cntmiss[i]++;//©�����
            }
        }
    }
    cout << "debug:ʵ����������\n";
    for (int i = 0; i < BIN; i++) {
        cout << cnterror[i] << endl;
    }
    cout << "debug:ʵ��©�������\n";
    for (int i = 0; i < BIN; i++) {
        cout << cntmiss[i] << endl;
    }
    for (int i = 0; i < BIN; i++) {
        pfa_experimental[i] = cnterror[i] * 1.0 / DECT;
        pm_experimental[i] = cntmiss[i] * 1.0 / DECT;
    }
    cout << "debug:ʵ������ʣ�\n";
    for (int i = 0; i < BIN; i++) {
        cout << pfa_experimental[i] << endl;
    }
    cout << "debug:ʵ��©���ʣ�\n";
    for (int i = 0; i < BIN; i++) {
        cout << pm_experimental[i] << endl;
    }
}
