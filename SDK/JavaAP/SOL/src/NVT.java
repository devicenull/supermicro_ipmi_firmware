import java.io.IOException;
import java.io.*;

public class NVT {
	private Network network;

	private final static byte ECHO = 1; 
	private final static byte SGA = 3; 
	private final static byte ST = 5; 
	private final static byte TM = 6; 
	private final static byte TT = 24; 
	private final static byte WS = 31; 
	private final static byte TS = 32; 
	private final static byte RFC = 33; 
	private final static byte LM = 34; 
	private final static byte XDL = 35; 
	private final static byte EV = 36; 
	private final static byte NEO = 39; 

	private final static byte SE = (byte) 240;
	private final static byte NOP = (byte) 241;
	private final static byte DM = (byte) 242;
	private final static byte BRK = (byte) 243;
	private final static byte IP = (byte) 244;
	private final static byte AO = (byte) 245;
	private final static byte AYT = (byte) 246;
	private final static byte EC = (byte) 247;
	private final static byte EL = (byte) 248;
	private final static byte GA = (byte) 249;
	private final static byte SB = (byte) 250;
	private final static byte WILL = (byte) 251;
	private final static byte WONT = (byte) 252;
	private final static byte DO = (byte) 253;
	private final static byte DONT = (byte) 254;
	private final static byte IAC = (byte) 255;

	private byte[] buf;
	private int bufpos, buflen;

	private void proc_sb() throws IOException {
		byte b, buf1, buf2;

		b = read();
		buf2 = read();

		while (true) {
			buf1 = buf2;
			buf2 = read();
			if (buf1 == IAC && buf2 == SE) {
				break;
			}
		}

		switch (b) {
		case TT:
			byte[] ttbuf = { 'v', 't', '1', '0', '0' };
			send_sb(b, (byte) 0, ttbuf, ttbuf.length);
			break;
		case TS:
			byte[] tsbuf = { '3', '8', '4', '0', '0', ',', '3', '8', '4', '0',
					'0' };
			send_sb(b, (byte) 0, tsbuf, tsbuf.length);
			break;
		default:
			break;
		}
	}

	private void proc_will() throws IOException {
		byte b;

		b = read();

		if (b == ECHO || b == SGA) {
			send_command(DO, b);
		} else {
			send_command(DONT, b);
		}

	}

	private void proc_wont() throws IOException {
		byte b;

		b = read();

		send_command(DONT, b);
	}

	private void proc_do() throws IOException {
		byte b;

		b = read();

		if (b == WS) {
			send_command(WILL, b);
			byte[] ws = { 0x50, 0x00, 0x18 };
			send_sb(b, (byte) 0, ws, 3);
		} else if (b == TT || b == TS) {
			send_command(WILL, b);
		} else if (b == ECHO) {
			send_command(WONT, b);
		} else {
			send_command(WONT, b);
		}
	}

	private void proc_dont() throws IOException {
		byte b;

		b = read();

		send_command(WONT, b);

	}

	private void proc_iac() throws IOException {
		byte b;

		b = read();
		switch (b) {
		case SB:
			proc_sb();
			break;
		case WILL:
			proc_will();
			break;
		case WONT:
			proc_wont();
			break;
		case DO:
			proc_do();
			break;
		case DONT:
			proc_dont();
			break;
		default:
			break;
		}
	}

	private void send_sb(byte opt1, byte opt2, byte[] opt3, int size)
			throws IOException {
		int i;
		byte[] buf = new byte[size + 6];

		buf[0] = IAC;
		buf[1] = SB;
		buf[2] = opt1;
		buf[3] = opt2;

		for (i = 0; i < size; i++) {
			buf[4 + i] = opt3[i];
		}

		buf[4 + size] = IAC;
		buf[5 + size] = SE;

		network.writeBytes(buf, 0, size + 6);
	}

	private void send_command(byte comm, byte opt) throws IOException {
		byte[] buf = new byte[3];

		buf[0] = IAC;
		buf[1] = comm;
		buf[2] = opt;

		network.writeBytes(buf, 0, 3);
	}

	private void fillBuf() throws IOException {
		buflen = network.readBytes(buf);
		bufpos = 0;
	}

	private byte read() throws IOException {
		while (bufpos == buflen) {
			fillBuf();
		}

		return buf[bufpos++];
	}

	public int readBytes(byte[] b) throws IOException {
		int len = 0;
		while (true) {

			if (bufpos == buflen && len == 0) {
				fillBuf();
				continue;
			}

			if (bufpos == buflen || len == b.length) {
				break;
			}

			if (buf[bufpos] == IAC) {
				bufpos++;
				proc_iac();

			} else {
				b[len++] = buf[bufpos++];

			}
		}

		return len;

	}

	public NVT(Network net) {
		network = net;

		buf = new byte[4096];
		bufpos = buflen = 0;

	}
}
