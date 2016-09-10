#include <iostream>
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>
using std::cout;
using std::endl;

//���������� Ÿ�̸� ����ϱ�
/* ���������� Ÿ�̸� ����ϱ�
- ��� ���α׷��� ��ϻ��·� ����ϰ� �ϴ°�

int main()
{
	//asio�� ����ϴ� ��� ���α׷��� ��� �ϳ��̻���
	// boost::asio::io_service ��ü�� ������ �־�� �Ѵ�
	// �� Ŭ������ I/O��ɿ� �����ϴ� ���� �����Ѵ�
	boost::asio::io_service io;

	//I/O����� �����ϴ� asio�� �ٽ� Ŭ��������
	// �׻� �������� ù ��° ���ڷ� io_service�� �������� ����
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	cout << "Sleep(2000)ȣ�� " 
		<< curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << endl;
	Sleep(2000);

	//Ÿ�̸Ӱ� ������ �� 5�ʰ� ������ Ÿ�̸Ӱ� �ױ�������
	// boost::asio::deadline_timer::wait() �� ���ϵ��� ����
	// *wait�� ȣ����� 5�ʰ� �ƴϴ�*

	//deadline Ÿ�̸Ӵ� �׻� �׾��ų�, �����ʾҰų� �� �� �ϳ��� ����.
	//���� Ÿ�̸Ӱ� �׾��ִٸ� wait()�� ȣ��Ǵ� ��� ���ϵǾ� ����.
	GetLocalTime(&curTime);
	cout << "t.wait()ȣ�� "
		<< curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << endl;
	t.wait();

	
	GetLocalTime(&curTime);
	cout << "t.wait()ȣ�� �� t�� ���ϵ� �ð� "
		<< curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << endl;
	std::cout << "Hell o World!" << std::endl;

	Sleep(4000);

	return 0;
}*/

//�񵿱������� Ÿ�̸� ����ϱ�
/* 
- asio�� �񵿱� �ݹ� ����� ����ϴ� ���

//�񵿱� ��Ⱑ ����� �� ȣ��Ǵ� �Լ�
void print(const boost::system::error_code&)
{
	std::cout << "Hell o World!" << std::endl;
}

int main()
{
	boost::asio::io_service io;

	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	cout << curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << endl;
	
	//�ݹ� �ڵ鷯�� �Ѱ���
	t.async_wait(print);

	//io_service�� run()����Լ��� �ݵ�� ȣ�����־���Ѵ�.
	//asio���̺귯������ �ݹ� �ڵ鷯��,
	//���� ���� boost::asio::io_service::run() �Լ��� ȣ���� ������κ���
	//ȣ��� ���̶�� ���� �������ش�.

	//run()�Լ��� ȣ����� �ʾҴٸ�, �񵿱� ����� �Ϸ� �ݹ��� ����� �Ͼ�� ����.
	//run()�Լ��� ���� ������ �����ִ� ���ȿ��� ��� �����
	//���⼭�� async_wait �񵿱� ���
	//run()�� ȣ���ϱ� ���� io_service�� ������ �ִ� ���� �߿�.
	io.run();

	GetLocalTime(&curTime);
	cout << curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << endl;
	Sleep(4000);

	return 0;
}*/

//���ڸ� �ڵ鷯�� ���ε��ϱ�
/* �ڵ鷯 �Լ��� �߰������� ���� �����ϱ�
	�񵿱������� �����ϴ� Ÿ�̸Ӹ�
	1�ʸ��� �ѹ��� �������� ����.
*/

void print(const boost::system::error_code& /*e*/,
	boost::asio::deadline_timer* t, int* count)
{
	//Ÿ�̸Ӹ� �ݺ������� ����ϱ� ���ؼ���
	//�翬�� �ݹ��Լ� ���ο��� Ÿ�̸ӿ� ���� ������ �� �־���ϰ�
	//�̸� ��Ʈ�� �ϱ����� ī���Ͱ� �ʿ��ϴ�.
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	cout << curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << " ";
	cout << "�ݹ��Լ� ���� count : " << *count << endl;

	if (*count < 5)
	{
		cout << *count << endl;
		++(*count);

		//Set the timer's expiry time as an absolute time.
		//return value : The number of asynchronous operations that were cancelled.
		//��, ���� ����ð����� ���� 1�ʸ� ���� �ð���ŭ ����ð��� ����
		t->expires_at(t->expires_at() + boost::posix_time::seconds(1));


		GetLocalTime(&curTime);
		cout <<"���ο� �񵿱��� ȣ���� - "
			<< curTime.wHour << ":"
			<< curTime.wMinute << ":"
			<< curTime.wSecond << endl;

		//async_wait() �Լ���
		//void(const boost::system::error_code&)������ �ڵ鷯�Լ�(Ȥ�� �Լ���ü)�� �޴´�.
		//boost::bind()�Լ��� �ݹ��Լ��� ������ �Ű������� ������Ű�µ� ���
		//��, print ��� �ڵ鷯�Լ��� async_wait�� �Ѱ���� �ϴµ�
		//print�� �߰��� �Ű������� �� �ʿ��ϹǷ� bind�� ���� ������(���ε�)
		//bind�Լ��� ���ε��ϴ� ������ ���Ŀ� �´� �Լ���ü�� ��ȯ�����ִ� ��
		t->async_wait(boost::bind(print,
			boost::asio::placeholders::error, t, count));
		//boosy::asio::placeholders::error�� �ڵ鷯�� ��������
		//���� ��ü�� ���� �̸��� �������ִ� ������.

		GetLocalTime(&curTime);
		cout <<"���ο� �񵿱��� ȣ������ ��� - "
			<< curTime.wHour << ":"
			<< curTime.wMinute << ":"
			<< curTime.wSecond << endl;
	}
}

int main()
{
	boost::asio::io_service io;

	int count = 0;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));

	t.async_wait(boost::bind(print,
		boost::asio::placeholders::error, &t, &count));
	io.run();

	cout << "Final count is" << count << endl;
	Sleep(10000);
	return 0;
}