#include <iostream>

using std::cout;
using std::endl;

class Problem {
public:
	// �R���X�g���N�^
	Problem(const char file_name[]) {
		cout << file_name << endl;
	}

	// ���g��\������
	void put() const noexcept{
		cout << "�y���t�@�C���z" << endl;
	}
};
