import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.UnknownHostException;

public class ClientSocket {

	public static void main(String[] args) {

		// TODO Auto-generated method stub
		String ip = "77.163.17.142";
		int port = 10000;
		java.net.Socket socket = null;
		try {
			socket = new java.net.Socket(ip, port);
		} catch (UnknownHostException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		try {

			OutputStreamWriter out = null;
			try {
				out = new OutputStreamWriter(socket.getOutputStream());
			} catch (IOException e2) {
				e2.printStackTrace();
			}
			InputStreamReader in = null;
			try {
				in = new InputStreamReader(socket.getInputStream());
			} catch (IOException e1) {
				e1.printStackTrace();
			}
			String authentication = "TEST";

			// send
			PrintWriter pw = new PrintWriter(out, true);
			pw.print(authentication);
			pw.flush();

			// receive initiate reply
			BufferedReader br = new BufferedReader(in);
			char[] buffer = new char[300];
			int count = 0;
			try {
				count = br.read(buffer, 0, 300);
			} catch (IOException e) {
				e.printStackTrace();
			}

			String reply = new String(buffer, 0, count);
			System.out.println(reply);

		} finally {
			try {
				socket.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

}
