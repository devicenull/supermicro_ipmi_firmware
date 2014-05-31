import java.io.*;
import java.net.*;

public class Network extends Thread {
	private String host;
	private int port;
	private int local_port;
	private DatagramSocket sock;

	private DatagramPacket spkt, rpkt, spkt_wb;
	byte buffer[] = new byte[4096];

	public static final String proto[] = { "telnet", "ssh" };
	public static final int TELNET = 0;
	public static final int SSH = 1;

	private int protocol;
	private boolean authenticated;
	private boolean connected, closed;
	
	private boolean dumpData = false;
	java.io.FileOutputStream out;

	public Network(String h, int p, int pr) {
		host = h;
		port = p;
		protocol = pr;

		if (protocol == SSH) {
			connected = false;
			authenticated = false;
			closed = false;
		}
	}

	public void setLocalPort(int port) {
		local_port = port;
	}
	
	public boolean connect() {

		if (protocol == TELNET) {
			try {
				sock = new DatagramSocket(local_port);
				sock.connect(InetAddress.getByName(host), port);
				//sock.setSoTimeout(30 * 1000);
				
				if(dumpData) {
					out = new FileOutputStream(new File("D:/random.txt"));
				}
				
			} catch (UnknownHostException e) {
				// 可能是未連線或連線位置錯誤
				System.out.println("Caught UnknownHostException in Network::connect()");
				return false;
			} catch (IOException e) {

				System.out.println("Caught IOException in Network::connect()");
				return false;
			}

			return true;

		} else {
			// Unknown protocol
			return false;
		}

	}

	public void disconnect() {
		if (protocol == TELNET) {
			// 如果跟本沒連線成功或是連線已被關閉則不做任何事。
			if (sock == null || sock.isClosed()) {
				return;
			}

			try {
				sock.close();
			} catch (Exception e) {
				System.out.println("Caught IOException in Network::disconnect()");
			}

			if (sock.isClosed()) {
				;
			} else {
				System.out.println("Disconnect failed!");
			}

		} else if (protocol == SSH) {

			if (authenticated) {
				try {

				} catch (Exception e) {
					System.out.println("Caught IOException in Network::disconnect()");
				}
			}

			closed = true;
		}
	}

	byte[] sol_wb = new byte[60];
	byte[] ipmi_msg = new byte[23];

	public void writeByte(byte b) throws IOException {
		int len_spkt = 0;
		byte buffer1[] = new byte[1];
		buffer1[0] = b;
		spkt = new DatagramPacket(buffer1, buffer1.length, InetAddress.getByName(host), port);
		len_spkt = spkt.getLength();
		sol_wb[0] = (byte) 0x06;
		sol_wb[1] = (byte) 0x00;
		sol_wb[2] = (byte) 0xff;
		sol_wb[3] = (byte) 0x07;
		sol_wb[4] = (byte) 0x06;
		sol_wb[5] = (byte) 0x01;

		sol_wb[6] = (byte) S_O_L.inbuffer_osr[24];
		sol_wb[7] = (byte) S_O_L.inbuffer_osr[25];
		sol_wb[8] = (byte) S_O_L.inbuffer_osr[26];
		sol_wb[9] = (byte) S_O_L.inbuffer_osr[27];
		sol_wb[10] = ++S_O_L.ipmi_msg_1[10];
		sol_wb[11] = (byte) 0x00;
		sol_wb[12] = (byte) 0x00;
		sol_wb[13] = (byte) 0x00;

		sol_wb[14] = (byte) Byte.parseByte(String.valueOf(len_spkt + 4));
		sol_wb[15] = (byte) 0x00;

		sol_wb[16] = S_O_L.buf[16]++;
		sol_wb[17] = (byte) 0x00;
		sol_wb[18] = (byte) 0x00;
		sol_wb[19] = (byte) 0x00;
		sol_wb[20] = b;

		int length = 21;
		spkt_wb = new DatagramPacket(sol_wb, length, InetAddress.getByName(host), port);

		sock.send(spkt_wb);

	}

	public void writeBytes(byte[] buf, int offset, int size) throws IOException {
		int len_spkt = 0;
		byte buffer1[] = new byte[buf.length];
		buffer1 = buf;
		spkt = new DatagramPacket(buf, buf.length, InetAddress.getByName(host), port);
		len_spkt = size;

		sol_wb[0] = (byte) 0x06;
		sol_wb[1] = (byte) 0x00;
		sol_wb[2] = (byte) 0xff;
		sol_wb[3] = (byte) 0x07;
		sol_wb[4] = (byte) 0x06;
		sol_wb[5] = (byte) 0x01;

		sol_wb[6] = (byte) S_O_L.inbuffer_osr[24];
		sol_wb[7] = (byte) S_O_L.inbuffer_osr[25];
		sol_wb[8] = (byte) S_O_L.inbuffer_osr[26];
		sol_wb[9] = (byte) S_O_L.inbuffer_osr[27];
		sol_wb[10] = ++S_O_L.ipmi_msg_1[10];
		sol_wb[11] = (byte) 0x00;
		sol_wb[12] = (byte) 0x00;
		sol_wb[13] = (byte) 0x00;

		sol_wb[14] = (byte) Byte.parseByte(String.valueOf(len_spkt + 4));
		sol_wb[15] = (byte) 0x00;

		sol_wb[16] = S_O_L.buf[16]++;
		sol_wb[17] = (byte) 0x00;
		sol_wb[18] = (byte) 0x00;
		sol_wb[19] = (byte) 0x00;

		for (int j = 0; j < len_spkt; j++) {
			sol_wb[20 + j] = (byte) buffer1[j];
//			System.out.print("0x" + Integer.toHexString(new Integer(buffer1[j])) + " ");
		}
//		System.out.println("");
		int length = 20 + len_spkt;
		spkt_wb = new DatagramPacket(sol_wb, length, InetAddress.getByName(host), port);

		sock.send(spkt_wb);
	}

	public int readBytes(byte[] b) throws IOException {
		int r;

		byte[] a = new byte[4096];
		rpkt = new DatagramPacket(a, a.length);

		sock.receive(rpkt);

		r = rpkt.getLength();

//		if (a[21] == (byte) 0x01)
//		{
//			r = 0;
//		}
//		else
		if (r == 20 && a[16] == (byte) 0x00)
		{ // 收到一個ACK datagram
			r = 0;
		}
		else
//		if (a[20] != 0)
		{
			byte[] c = new byte[20];

			spkt = new DatagramPacket(c, c.length, InetAddress.getByName(host), port);
			c[0] = (byte) 0x06;
			c[1] = (byte) 0x00;
			c[2] = (byte) 0xff;
			c[3] = (byte) 0x07;
			c[4] = (byte) 0x06;
			// 由0x00改成0x01, 做ACK
			c[5] = (byte) 0x01;

			c[6] = (byte) S_O_L.inbuffer_osr[24];
			c[7] = (byte) S_O_L.inbuffer_osr[25];
			c[8] = (byte) S_O_L.inbuffer_osr[26];
			c[9] = (byte) S_O_L.inbuffer_osr[27];

			c[10]++;// 同步
			c[11] = (byte) 0x00;
			c[12] = (byte) 0x00;
			c[13] = (byte) 0x00;

			c[14] = (byte) 0x04;// message length
			c[15] = (byte) 0x00;
			c[16] = (byte) 0x00;
			c[17] = a[16];
			c[18] = (byte) (r - 20);
			c[19] = (byte) 0x04;// 20;
			sock.send(spkt);
			for (int i = 0; i < r - 20; i++) {
				b[i] = a[20 + i];
				//System.out.print("0x" + Integer.toHexString(new Integer(b[i])) + " ");
			}
			//System.out.println("");
			if (dumpData) {
				out.write(b, 0, r - 20);
				out.flush();
			}

			r = r - 20;
		}

		if (r == -1) {
			throw new IOException();
		}
		return r;
	}

	public boolean isClosed() {

		if (protocol == TELNET) {
			// XXX: 連線建立失敗也當成已關閉連線。
			if (sock == null) {
				return true;
			}

			return sock.isClosed();
		} else {
			return closed;
		}
	}

	public boolean isConnected() {
		if (protocol == TELNET) {
			if (sock == null) {
				return false;
			}
			return sock.isConnected();
		} else {
			return connected;
		}
	}
}