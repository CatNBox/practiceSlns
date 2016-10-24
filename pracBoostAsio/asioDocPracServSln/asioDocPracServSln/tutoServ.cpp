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

//�������� TCP Daytime Server
/*

int main()
{
	try
	{
		boost::asio::io_service ioServ;

		//���ο� Ŀ�ؼ��� listen�ϱ� ���ؼ�
		// boost::asio::ip:tcp::acceptor��ü�� �ʿ�
		tcp::acceptor acceptor(ioServ, tcp::endpoint(tcp::v4(), 13));

		//�� ������ itorative server
		// ��, �ѹ��� �ϳ����� Ŀ�ؼ��� ó����
		// Ŭ���̾�Ʈ�� Ŀ�ؼ��� ǥ���� ������ �ϳ� ����� ���
		//Ŭ���̾�Ʈ�� ����Ǹ� ����ð��� ���� ������ Ŭ���̾�Ʈ�� ����
		for (;;)
		{
			tcp::socket socket(ioServ);
			acceptor.accept(socket);

			std::string message = makeDaytimeStr();

			boost::system::error_code ignoredErr;
			boost::asio::write(socket, boost::asio::buffer(message),
				boost::asio::transfer_all(), ignoredErr);

			//����� ���Ͽ��� IP�� ����
			std::string remoteIP = socket.remote_endpoint().address().to_string();
			std::cout << "�����ð��� ��û�� IP : " << remoteIP << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
*/

//�񵿱����� TCP Daytime Server
/**/

//tcpConnection ��ü�� �����Ǵ� ���� ��ü�� ����α� ����
// shared_ptr�� enable_shared_from_this�� ��� - ����߰�
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

	//start() �Լ����� Ŭ���̾�Ʈ���� �����͸� �����ϱ�����
	// boost::asio::async_write()�Լ��� ȣ��
	//** boost::asio::async_write() �Լ��� boost::asio::ip::tcp::socket::async_write_some()�� ������ ��
	void start()
	{

		//����� ���Ͽ��� IP�� ����
		std::string remoteIP = socket_.remote_endpoint().address().to_string();
		std::cout << "�����ð��� ��û�� IP : " << remoteIP << std::endl;

		//������ �����͵��� �񵿱����� ���۷��̼��� �Ϸ�Ǵ� ����
		// �Ѽյ��� �ʵ��� message_��������� ����
		message_ = makeDaytimeStr();

		//bind()�� ����ؼ� "�񵿱����� ���۷��̼�"�� �ʱ�ȭ�� ��,
		// �ݵ�� �ڵ鷯�� �Ű����� ��Ͽ� ���� ������ ���ڵ��� �������־���Ѵ�
		//��α� - async_write()�� ������ �˾Ƶ� ��
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

	//Ŭ���̾�Ʈ���� Ŀ�ؼǿ� ���� ������ å���� handle_write() �Լ��� ����
	//handle_write()���� error�� byte_transferred �Ű������� ������ �ʱ⶧���� �����ϴ� ���� ����
	// �������ʴ� �Ű������� �����ϸ� start()�Լ��� aync_write�� bind�κ� ����
	// boost::bind(&tcpConnection::handle_write, shared_from_this())�� ������ �� �ִ�.
	void handle_write(const boost::system::error_code& , size_t )
	{
	}

	tcp::socket socket_;
	std::string message_;
};

class tcp_server
{
public:
	//�����ڿ��� listen�� ���� ����͸� TCP 13�� ��Ʈ�� �ʱ�ȭ
	tcp_server(boost::asio::io_service& ioService)
		: acceptor_(ioService, tcp::endpoint(tcp::v4(), 13))
	{
		start_accept();
	}
		
private:
	tcp::acceptor acceptor_;

	//start_accept() �Լ��� ������ �ϳ� �����ϰ�, ���ο� Ŀ�ؼ��� ��ٸ��� �񵿱� ���Ʈ�� ����
	void start_accept()
	{
		tcpConnection::pointer newConnection = tcpConnection::create(acceptor_.get_io_service());

		acceptor_.async_accept(newConnection->socket(),
			boost::bind(&tcp_server::handle_accept, this, newConnection,
				boost::asio::placeholders::error));
	}

	//handle_accept()�Լ��� �񵿱����� ���Ʈ�� ������ start_accept()�Լ��� ����� ���ؼ� �ʱ�ȭ�� �� ȣ��
	// Ŭ���̾�Ʈ�� ��û�� �����ϰ�, ���� ���Ʈ�� ������ ���� start_accept()�Լ��� ȣ��
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
		//Ŭ���̾�Ʈ�κ��� ������ ������ ���Ʈ�ϱ� ���ؼ� ���� ��ü�� ����
		//boost::asio::io_service ��ü�� ���ϰ� ���� ��ü�� IO���񽺸� �������ֱ� ���� ������ü�� ���� �� �ִ�
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