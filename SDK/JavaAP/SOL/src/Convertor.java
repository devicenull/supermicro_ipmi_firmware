import java.io.IOException;
import java.io.InputStream;

public class Convertor {

	private byte[] big5bytes;
	private byte[] ucs2bytes;
	private char[] ucs2chars;

	public void ucs2ToBig5Bytes(char c, byte[] b) {
		if (c < 0x80) {
			b[0] = (byte) c;
			return;
		}

		b[0] = big5bytes[(c - 0x80) * 2];
		b[1] = big5bytes[(c - 0x80) * 2 + 1];
	}

	public char big5BytesToUCS2(byte b1, byte b2) {
		int i1 = (b1 < 0 ? 256 : 0) + (int) b1;
		int i2 = (b2 < 0 ? 256 : 0) + (int) b2;

		int shift = ((i1 << 8) | i2) - 0x8140;

		if (shift < 0 || shift * 2 + 1 >= ucs2bytes.length) {
			return '?';
		}

		i1 = (ucs2bytes[shift * 2] < 0 ? 256 : 0) + (int) ucs2bytes[shift * 2];
		i2 = (ucs2bytes[shift * 2 + 1] < 0 ? 256 : 0)
				+ (int) ucs2bytes[shift * 2 + 1];

		return (char) (i1 << 8 | i2);
	}

	private int readFile(String name, byte[] b) {
		int begin = 0, len;
		InputStream is = S_O_L.class.getResourceAsStream(name);

		try {
			while (true) {
				len = is.read(b, begin, b.length - begin);
				if (len == -1) {
					break;
				}
				begin += len;
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

		return begin;
	}

	public Convertor() {
		ucs2bytes = new byte[64 * 1024];
		big5bytes = new byte[128 * 1024];

		readFile("conv/ucs2.txt", ucs2bytes);
		readFile("conv/big5.txt", big5bytes);

		ucs2chars = new char[ucs2bytes.length / 2];

		for (int i = 0; i < ucs2bytes.length; i += 2) {
			int i1 = (ucs2bytes[i] < 0 ? 256 : 0) + (int) ucs2bytes[i];
			int i2 = (ucs2bytes[i + 1] < 0 ? 256 : 0) + (int) ucs2bytes[i + 1];
			
			ucs2chars[i / 2] = (char) (i1 << 8 | i2);
		}
	}
}
