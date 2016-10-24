#pragma warning(disable:4996)

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp> 
#include <boost/enable_shared_from_this.hpp> 
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string makeDaytimeStr()
{
	using namespace std; // for time_t, time, ctime
	time_t now = time(0);
	return ctime(&now);
}

//동기적인 TCP Daytime Server
/*

int main()
{
	try
	{
		boost::asio::io_service ioServ;

		//새로운 커넥션을 listen하기 위해서
		// boost::asio::ip:tcp::acceptor객체가 필요
		tcp::acceptor acceptor(ioServ, tcp::endpoint(tcp::v4(), 13));

		//이 서버는 itorative server
		// 즉, 한번에 하나씩의 커넥션을 처리함
		// 클라이언트와 커넥션을 표현할 소켓을 하나 만들고 대기
		//클라이언트가 연결되면 현재시간에 대한 정보를 클라이언트에 전송
		for (;;)
		{
			tcp::socket socket(ioServ);
			acceptor.accept(socket);

			std::string message = makeDaytimeStr();

			boost::system::error_code ignoredErr;
			boost::asio::write(socket, boost::asio::buffer(message),
				boost::asio::transfer_all(), ignoredErr);

			//연결된 소켓에서 IP를 추출
			std::string remoteIP = socket.remote_endpoint().address().to_string();
			std::cout << "서버시간을 요청한 IP : " << remoteIP << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
*/

//비동기적인 TCP Daytime Server
/**/

//tcpConnection 객체가 참조되는 동안 객체를 살려두기 위해
// shared_ptr과 enable_shared_from_this를 사용 - 헤더추가
class tcpConnection
	: public boost::enable_shared_from_this<tcpConnection>
{
public:
	typedef boost::shared_ptr<tcpConnection> pointer;

	static pointer create(boost::asio::io_service& ioService)
	{
		return pointer(new tcpConnection(ioService));
	}

	tcp::socket& socket()
	{
		return socket_;
	}

	//start() 함수에서 클라이언트에게 데이터를 제공하기위해
	// boost::asio::async_write()함수를 호출
	//** boost::asio::async_write() 함수와 boost::asio::ip::tcp::socket::async_write_some()을 구분할 것
	void start()
	{

		//연결된 소켓에서 IP를 추출
		std::string remoteIP = socket_.remote_endpoint().address().to_string();
		std::cout << "서버시간을 요청한 IP : " << remoteIP << std::endl;

		//보내질 데이터들은 비동기적인 오퍼레이션이 완료되는 동안
		// 훼손되지 않도록 message_멤버변수에 저장
		message_ = makeDaytimeStr();

		//bind()를 사용해서 "비동기적인 오퍼레이션"이 초기화될 때,
		// 반드시 핸들러의 매개변수 목록에 가장 적합한 인자들을 지정해주어야한다
		//블로그 - async_write()의 구조를 알아둘 것
		boost::asio::async_write(socket_, boost::asio::buffer(message_),
			boost::bind(&tcpConnection::handle_write, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

private:
	tcpConnection(boost::asio::io_service& ioService)
		:socket_(ioService)
	{
	}

	//클라이언트와의 커넥션에 대한 나머지 책임은 handle_write() 함수에 있음
	//handle_write()에서 error와 byte_transferred 매개변수가 사용되지 않기때문에 제거하는 것이 가능
	// 사용되지않는 매개변수를 제거하면 start()함수의 aync_write의 bind부분 또한
	// boost::bind(&tcpConnection::handle_write, shared_from_this())로 변경할 수 있다.
	void handle_write(const boost::system::error_code& , size_t )
	{
	}

	tcp::socket socket_;
	std::string message_;
};

class tcp_server
{
public:
	//생성자에서 listen을 위한 억셉터를 TCP 13번 포트로 초기화
	tcp_server(boost::asio::io_service& ioService)
		: acceptor_(ioService, tcp::endpoint(tcp::v4(), 13))
	{
		start_accept();
	}
		
private:
	tcp::acceptor acceptor_;

	//start_accept() 함수는 소켓을 하나 생성하고, 새로운 커넥션을 기다리는 비동기 억셉트를 수행
	void start_accept()
	{
		tcpConnection::pointer newConnection = tcpConnection::create(acceptor_.get_io_service());

		acceptor_.async_accept(newConnection->socket(),
			boost::bind(&tcp_server::handle_accept, this, newConnection,
				boost::asio::placeholders::error));
	}

	//handle_accept()함수는 비동기적인 억셉트의 수행이 start_accept()함수의 종료로 인해서 초기화될 때 호출
	// 클라이언트의 요청에 응답하고, 다음 억셉트의 수행을 위해 start_accept()함수를 호출
	void handle_accept(tcpConnection::pointer newConnection, const boost::system::error_code& error)
	{
		if (!error)
		{
			newConnection->start();
			start_accept();
		}
	}

};

int main()
{
	try
	{
		//클라이언트로부터 들어오는 접속을 억셉트하기 위해서 서버 객체를 생성
		//boost::asio::io_service 객체는 소켓과 같은 객체에 IO서비스를 제공해주기 위해 서버객체로 사용될 수 있다
		boost::asio::io_service ioService;
		tcp_server server(ioService);

		ioService.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
//*/