#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

//�������� TCP DAYTIME CLIENT
/*
	TCPŬ���̾�Ʈ ���ø����̼� ��������
	
	�� ���ø����̼��� ������ DAYTIME���񽺿� �����ϴ� ��
*/
using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	//������ ����
	try
	{
		if (argc != 2)
		{
			std::cerr << "Usage : Client <host>" << std::endl;
			return 1;
		}
		
		//asio�� ����ϴ� ���α׷��� ��� �ϳ��� boost::asio::io_service�� �������ϹǷ�
		boost::asio::io_service ioServ;

		//boost::asio::ip::tcp::resolver ��ü�� ����ϱ� ���ؼ�
		// ���ø����̼��� �Ű������� ���ǵ� ������ �̸��� TCP�ܸ���(endpoint)���� ��ȯ�ؾ��Ѵ�
		tcp::resolver resolver(ioServ);

		//������(resolver)�� ���� ������Ʈ�� �޾Ƽ� �ܸ����� ������� ��ȯ.
		//���⼭ argv[1]���� ���ǵ� ������ �̸��� "daytime"�̶�� ������ �̸����� ������ ����.
		tcp::resolver::query query(argv[1], "daytime");

		//�ܸ��� ����� boost::asio::ip::tcp::resolver::iterator Ÿ���� ��ȯ�ȴ�.
		//iterator ��ü�� �⺻�����ڴ� ����Ʈ�� ���� ����Ű�� �ݺ��ڸ� ���� �����ȴ�.
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		//�ܸ��� ����� IPv4�� IPv6�� ������� �ܸ����� �����ϰ�����.
		//������ ��� ���� �´��� ã�ƺ��ƾ� �ϸ�, �̰��� Ŭ���̾�Ʈ�� ���ǵ� IP������ �������̰� ��.
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
			//���� ������ ������ ���� ����
			boost::array<char, 128> buf;
			boost::system::error_code errCode;

			size_t len = socket.read_some(boost::asio::buffer(buf), errCode);

			//������ Ŀ�ؼ��� ������ boost::asio::ip:tcp::socket::read_some() �Լ���
			//boost::asio::error::eof ������ �Բ� �����
			if (errCode == boost::asio::error::eof)
			{
				break; //peer�� ���� ���������� ����
			}
			else if (errCode)
			{
				throw boost::system::system_error(errCode); // ������ ���ᰡ �ƴ� ����
			}

			std::cout.write(buf.data(), len);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}