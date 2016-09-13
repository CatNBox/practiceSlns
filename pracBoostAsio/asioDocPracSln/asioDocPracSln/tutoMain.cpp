#include <iostream>
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\thread.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>
using std::cout;
using std::endl;

//동기적으로 타이머 사용하기
/* 동기적으로 타이머 사용하기
- 어떻게 프로그램을 블록상태로 대기하게 하는가

int main()
{
	//asio를 사용하는 모든 프로그램은 적어도 하나이상의
	// boost::asio::io_service 객체를 가지고 있어야 한다
	// 이 클래스는 I/O기능에 접근하는 것을 제공한다
	boost::asio::io_service io;

	//I/O기능을 제공하는 asio의 핵심 클래스들은
	// 항상 생성자의 첫 번째 인자로 io_service의 참조값을 받음
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	cout << "Sleep(2000)호출 " 
		<< curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << endl;
	Sleep(2000);

	//타이머가 생성된 지 5초가 지나서 타이머가 죽기전까지
	// boost::asio::deadline_timer::wait() 은 리턴되지 않음
	// *wait이 호출된지 5초가 아니다*

	//deadline 타이머는 항상 죽었거나, 죽지않았거나 둘 중 하나의 상태.
	//만약 타이머가 죽어있다면 wait()이 호출되는 즉시 리턴되어 버림.
	GetLocalTime(&curTime);
	cout << "t.wait()호출 "
		<< curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << endl;
	t.wait();

	
	GetLocalTime(&curTime);
	cout << "t.wait()호출 후 t가 리턴된 시간 "
		<< curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << endl;
	std::cout << "Hell o World!" << std::endl;

	Sleep(4000);

	return 0;
}*/

//비동기적으로 타이머 사용하기
/* 
- asio의 비동기 콜백 기능을 사용하는 방법

//비동기 대기가 종료될 때 호출되는 함수
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
	
	//콜백 핸들러를 넘겨줌
	t.async_wait(print);

	//io_service의 run()멤버함수를 반드시 호출해주어야한다.
	//asio라이브러리에서 콜백 핸들러는,
	//오직 현재 boost::asio::io_service::run() 함수를 호출한 스레드로부터
	//호출될 것이라는 것을 보장해준다.

	//run()함수가 호출되지 않았다면, 비동기 대기의 완료 콜백은 절대로 일어나지 않음.
	//run()함수는 아직 할일이 남아있는 동안에는 계속 수행됨
	//여기서는 async_wait 비동기 대기
	//run()을 호출하기 전에 io_service에 할일을 주는 것이 중요.
	io.run();

	GetLocalTime(&curTime);
	cout << curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << endl;
	Sleep(4000);

	return 0;
}*/

//인자를 핸들러에 바인딩하기
/* 핸들러 함수에 추가적으로 인자 전달하기
	비동기적으로 동작하는 타이머를
	1초마다 한번씩 켜지도록 구현.


void print(const boost::system::error_code& ,
	boost::asio::deadline_timer* t, int* count)
{
	//타이머를 반복적으로 사용하기 위해서는
	//당연히 콜백함수 내부에서 타이머에 대해 접근할 수 있어야하고
	//이를 컨트롤 하기위해 카운터가 필요하다.
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	cout << curTime.wHour << ":"
		<< curTime.wMinute << ":"
		<< curTime.wSecond << " ";
	cout << "콜백함수 진입 count : " << *count << endl;

	if (*count < 5)
	{
		cout << *count << endl;
		++(*count);

		//Set the timer's expiry time as an absolute time.
		//return value : The number of asynchronous operations that were cancelled.
		//즉, 이전 종료시간으로 부터 1초를 더한 시간만큼 종료시간을 설정
		t->expires_at(t->expires_at() + boost::posix_time::seconds(1));


		GetLocalTime(&curTime);
		cout <<"새로운 비동기대기 호출전 - "
			<< curTime.wHour << ":"
			<< curTime.wMinute << ":"
			<< curTime.wSecond << endl;

		//async_wait() 함수는
		//void(const boost::system::error_code&)형식의 핸들러함수(혹은 함수객체)를 받는다.
		//boost::bind()함수는 콜백함수에 여분의 매개변수를 연관시키는데 사용
		//즉, print 라는 핸들러함수를 async_wait에 넘겨줘야 하는데
		//print는 추가로 매개변수가 더 필요하므로 bind를 통해 묶어줌(바인딩)
		//bind함수로 바인딩하는 것으로 형식에 맞는 함수객체로 변환시켜주는 것
		t->async_wait(boost::bind(print,
			boost::asio::placeholders::error, t, count));
		//boost::asio::placeholders::error는 핸들러에 전해지는
		//에러 객체를 위한 이름이 정해져있는 지정자.

		GetLocalTime(&curTime);
		cout <<"새로운 비동기대기 호출직후 블록 - "
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

	cout << "Final count is " << count << endl;
	Sleep(10000);
	return 0;
}*/

//멤버함수를 핸들러로 사용하기
/*
	클래스 멤버 함수를 어떻게 콜백핸들러로 사용하는가
	프로그램 자체의 기능은 인자를 핸들러에 바인딩하기 예제와 같음


class printer
{
public:
	//이 클래스의 생성자는 io_service의 참조값을 받아서
	//timer_ 멤버를 초기화할 때 사용
	printer(boost::asio::io_service& io)
		: timer_(io, boost::posix_time::seconds(1)),
		count_(0)
	{
		//boost::bind() 함수는 자유함수, 클래스멤버함수 모두 잘 작동
		//모든 비 정적(non-static)멤버함수들은 암시적으로 this 매개변수를 가지고 있기 때문에,
		// 함수에 this를 함께 바인딩 해야한다

		//print멤버함수는 매개변수로 에러 객체를 받지 않기 때문에
		//여기서는 boost::asio::placeholders::error placeholder 가 명시되지않음
		timer_.async_wait(boost::bind(&printer::print,this));
	}

	~printer()
	{
		cout << "Final count is " << count_ << endl;
		Sleep(10000);
	}

	void print()
	{
		if (count_ < 5)
		{
			cout << count_ << endl;
			count_++;

			timer_.expires_at(timer_.expires_at() + boost::posix_time::seconds(1));
			timer_.async_wait(boost::bind(&printer::print, this));
		}
	}

private:
	boost::asio::deadline_timer timer_;
	int count_;
};

int main()
{
	boost::asio::io_service io;
	printer p(io);
	io.run();

	return 0;
}*/

//멀티스레드 프로그램에 핸들러를 동기화하기
/*
*/