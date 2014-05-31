import java.util.Date;
import java.util.Iterator;
import java.util.Vector;

public class Site {

	public String name;

	public String host;
	public int port;

	public int protocol;

	public String alias;

	public boolean autoconnect, autologin;

	public String prelogin, postlogin;

	public String usernameprompt, username, userpassprompt, userpass;

	public Date lastvisit;

	int order;

	public String toString() {
		Vector v = new Vector();

		v.addElement("name=" + name);
		v.addElement("host=" + host);
		v.addElement("port=" + new Integer(port).toString());
		v.addElement("protocol=" + Network.proto[protocol]);
		v.addElement("alias=" + alias);

		if (autoconnect) {
			v.addElement("autoconnect=true");
		} else {
			v.addElement("autoconnect=false");
		}

		return CSV.generate(v);
	}

	public Site(String n, String h, int p) {
		name = n;
		host = h;
		port = p;

		protocol = Network.TELNET;
		alias = "";
		autoconnect = false;
		autologin = false;
	}

	public Site(String h) {
		Vector v = CSV.parse(h);
		Iterator iter = v.iterator();
		String s, key, value;
		int i;

		protocol = Network.TELNET;
		alias = "";
		autoconnect = false;
		autologin = false;

		while (iter.hasNext()) {
			s = (String) iter.next();
			i = s.indexOf("=");

			key = s.substring(0, i);
			value = s.substring(i + 1);

			if (key.equalsIgnoreCase("name")) {
				name = value;
			} else if (key.equalsIgnoreCase("host")) {
				host = value;
			} else if (key.equalsIgnoreCase("port")) {
				port = Integer.parseInt(value);
			} else if (key.equalsIgnoreCase("protocol")) {
				for (int j = 0; j < Network.proto.length; j++) {
					if (value.equalsIgnoreCase(Network.proto[j])) {
						protocol = j;
						break;
					}
				}
			} else if (key.equalsIgnoreCase("alias")) {
				alias = value;
			} else if (key.equalsIgnoreCase("autoconnect")) {
				autoconnect = value.equalsIgnoreCase("true");
			}
		}
	}

}
