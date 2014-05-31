import java.util.Iterator;
import java.util.Vector;

public class CSV {

	public static Vector parse(String s) {
		Vector v = new Vector();
		StringBuffer buf;
		int i, state;
		char c;

		buf = new StringBuffer();
		for (i = state = 0; i < s.length(); i++) {

			c = s.charAt(i);

			switch (state) {
			case 0:
				if (c == '"') {
					state = 1;
				}
				break;
			case 1:
				if (c == '"') {
					state = 2;
				} else {
					buf.append(c);
					state = 3;
				}
				break;
			case 2:
				if (c == ',') {
					v.addElement(new String(buf));
					buf.delete(0, buf.length());
					state = 0;
				} else if (c == '"') {
					buf.append(c);
					state = 3;
				}
				break;
			case 3:
				if (c == '"') {
					state = 2;
				} else {
					buf.append(c);
				}
				break;
			}
		}

		if (buf.length() != 0) {
			v.addElement(new String(buf));
		}

		return v;
	}

	public static String generate(Vector v) {
		String s;
		StringBuffer sb = new StringBuffer();
		Iterator iter = v.iterator();

		while (iter.hasNext()) {
			s = (String) iter.next();
			sb.append("\"" + s.replaceAll("\"", "\"\"") + "\",");
		}

		return new String(sb);
	}
}
