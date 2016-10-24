#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

//동기적인 TCP DAYTIME CLIENT
/*
	TCP클라이언트 어플리케이션 구현예제
	
	이 어플리케이션의 목적은 DAYTIME서비스에 접속하는 것
*/
using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	//서버를 지정
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage : Client <host>" << std::endl;
			return 1;
		}
		
		//asio를 사용하는 프로그램은 적어도 하나의 boost::asio::io_service를 가져야하므로
		boost::asio::io_service ioServ;

		//boost::asio::ip::tcp::resolver 객체를 사용하기 위해서
		// 어플리케이션의 매개변수로 정의된 서버의 이름을 TCP단말점(endpoint)으로 변환해야한다
		tcp::resolver resolver(ioServ);

		//결정자(resolver)는 쿼리 오브젝트를 받아서 단말점의 목록으로 변환.
		//여기서 argv[1]에서 정의된 서버의 이름과 "daytime"이라는 서비스의 이름으로 쿼리를 생성.
		tcp::resolver::query query(argv[1], "daytime");

		//단말점 목록은 boost::asio::ip::tcp::resolver::iterator 타입의 반환된다.
		//iterator 객체의 기본생성자는 리스트의 끝을 가리키는 반복자를 통해 생성된다.
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		//단말점 목록은 IPv4와 IPv6에 상관없이 단말점을 포함하고있음.
		//때문에 어느 것이 맞는지 찾아보아야 하며, 이것은 클라이언트가 정의된 IP버전에 독립적이게 됨.
		tcp::socket socket(ioServ);
		boost::system::error_code errCode = boost::asio::error::host_not_found;
		while (errCode && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, errCode);
		}

		if (errCode)
		{
			throw boost::system::system_error(errCode);
		}

		for (;;)
		{
			//받은 데이터 저장을 위한 버퍼
			boost::array<char, 128> buf;
			boost::system::error_code errCode;

			size_t len = socket.read_some(boost::asio::buffer(buf), errCode);

			//서버가 커넥션을 닫으면 boost::asio::ip:tcp::socket::read_some() 함수는
			//boost::asio::error::eof 에러와 함께 종료됨
			if (errCode == boost::asio::error::eof)
			{
				break; //peer에 의해 정상적으로 종료
			}
			else if (errCode)
			{
				throw boost::system::system_error(errCode); // 정상적 종료가 아닌 에러
			}

			std::cout.write(buf.data(), len);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}