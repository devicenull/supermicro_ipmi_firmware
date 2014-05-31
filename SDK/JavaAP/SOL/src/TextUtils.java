public class TextUtils {

	public static String fmt(String str, int width) {
		char c, c2;
		int count = 0;
		StringBuffer sb = new StringBuffer();

		for (int i = 0; i < str.length(); i++) {
			c = str.charAt(i);

			if (c > 127) {
				count += 2;
				sb.append(c);
			} else if (c == 0x09) {
				count += 4;
				sb.append(c);
			} else if (c == 0x0a || c == 0x0d) {
				c = 0x0d;
				count = 0;
				sb.append(c);
			} else {
				count++;
				sb.append(c);
			}

			if (count > width) {

				for (int j = sb.length() - 1; j >= 0; j--) {
					c2 = sb.charAt(j);
					if (c2 > 127) {
						sb.insert(j, (char) 0x0d);
						count = sb.length() - j - 1;
						break;
					} else if (c2 == ' ') {
						sb.setCharAt(j, (char) 0x0d);
						count = sb.length() - j - 1;
						break;
					} else if (c2 == (char) 0x0d) {
						break;
					}
				}
			}
		}

		return new String(sb);
	}
}
