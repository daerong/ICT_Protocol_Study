#include<iostream>
#include "Judgment.h"

//#include "2013182043_���ڷ�.h"
//#include "2013182044_�豸��.h"
#include "2011144024_���뼺.h"
//#include "2013180010_������.h"
//#include "2017184037_ȫ����.h"
//#include "2015182003_��ȣ��.h"
//#include "2013182024_�۹μ�.h"

using namespace std;

// �� ĭ �� 3, 4 ����

int main()
{
	Judgment judgement;

	judgement.SetYourFunc(AttackBlack_2011144024/*����*/, DefenceBlack_2011144024, WhiteAttack_2011144024/*�İ�*/, WhiteDefence_2011144024);
	judgement.GamePlay();

}