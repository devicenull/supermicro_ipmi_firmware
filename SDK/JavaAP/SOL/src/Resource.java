import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Iterator;
import java.util.TreeSet;
import java.util.Vector;

public class Resource {
	public static final String GEOMETRY_X = "geometry.x";
	public static final String GEOMETRY_Y = "geometry.y";
	public static final String GEOMETRY_WIDTH = "geometry.width";
	public static final String GEOMETRY_HEIGHT = "geometry.height";

	public static final String TERMINAL_ROWS = "terminal.rows";
	public static final String TERMINAL_COLUMNS = "terminal.columns";

	public static final String ANTI_IDLE = "connect.anti-idle";
	public static final String ANTI_IDLE_INTERVAL = "connect.anti-idle-interval";
	public static final String ANTI_IDLE_STRING = "connect.anti-idle-string";

	public static final String AUTO_RECONNECT = "connect.auto-reconnect";
	public static final String AUTO_RECONNECT_TIME = "connect.autoreconnect-time";
	public static final String AUTO_RECONNECT_INTERVAL = "connect.auto-reconnect-interval";

	public static final String FONT_SIZE = "font.size";
	public static final String FONT_USE_CUSTOM_FONT = "font.use-custom-font";
	public static final String FONT_TTF_PATH = "font.ttf-path";
	public static final String FONT_VERTICLAL_GAP = "font.vertical-gap";
	public static final String FONT_HORIZONTAL_GAP = "font.hoizontal-gap";
	public static final String FONT_DESCENT_ADJUST = "font.descent-adjust";

	public static final String EXTERNAL_BROWSER = "external-browser-command";
	public static final String SYSTEM_LOOK_FEEL = "use-system-look-and-feel";
	public static final String COPY_ON_SELECT = "copy-on-select";
	public static final String CLEAR_AFTER_COPY = "clear-after-copy";
	public static final String REMOVE_MANUAL_DISCONNECT = "remove-manual-disconnect";
	public static final String AUTO_LINE_BREAK = "auto-line-break";
	public static final String AUTO_LINE_BREAK_LENGTH = "auto-line-break-length";
	public static final String USE_CUSTOM_BELL = "use-custom-bell";
	public static final String CUSTOM_BELL_PATH = "custom-bell-path";
	public static final String SHOW_TOOLBAR = "show-toolbar";

	private HashMap map;

	private void parseLine(String line) {
		String[] argv;

		argv = line.split("::");

		if (argv.length != 2) {
			return;
		}

		if (argv[0].length() > 0) {
			map.put(argv[0], argv[1]);
		}
	}

	public void initValue() {
		map = new HashMap();

		setValue(GEOMETRY_X, "0");
		setValue(GEOMETRY_Y, "0");
		setValue(GEOMETRY_WIDTH, "980");
		setValue(GEOMETRY_HEIGHT, "720");
		setValue(SHOW_TOOLBAR, "true");

		setValue(TERMINAL_COLUMNS, "80");
		setValue(TERMINAL_ROWS, "24");

		setValue(ANTI_IDLE, "true");
		setValue(ANTI_IDLE_INTERVAL, "120");
		setValue(ANTI_IDLE_STRING, "");

		setValue(AUTO_RECONNECT, "true");
		setValue(AUTO_RECONNECT_TIME, "10");
		setValue(AUTO_RECONNECT_INTERVAL, "500");

		setValue(FONT_USE_CUSTOM_FONT, "false");
		setValue(FONT_SIZE, "0");
		setValue(FONT_TTF_PATH, "");
		setValue(FONT_VERTICLAL_GAP, "0");
		setValue(FONT_HORIZONTAL_GAP, "0");
		setValue(FONT_DESCENT_ADJUST, "0");

		setValue(EXTERNAL_BROWSER, "explorer \"%u\"");
		setValue(SYSTEM_LOOK_FEEL, "false");
		setValue(COPY_ON_SELECT, "false");
		setValue(CLEAR_AFTER_COPY, "true");
		setValue(REMOVE_MANUAL_DISCONNECT, "true");
		setValue(AUTO_LINE_BREAK, "false");
		setValue(AUTO_LINE_BREAK_LENGTH, "72");
		setValue(USE_CUSTOM_BELL, "false");
		setValue(CUSTOM_BELL_PATH, "");

		setValue("favorite.0",
				"\"name=動力核心\",\"host=cpu.tfcis.org\",\"port=23\"");
		setValue("favorite.1", "\"name=ptt\",\"host=ptt.cc\",\"port=23\"");
		setValue("favorite.2", "\"name=ptt2\",\"host=ptt2.cc\",\"port=23\"");
	}

	public File getRcFile() {
		File f;
		String home = System.getProperty("user.home");
		String rcfile = ".S_O_L rc";

		f = new File(home + File.separator + rcfile);
		return f;
	}

	public void readFile() {
		File rc = getRcFile();
		BufferedReader br;
		String buf;

		if (!rc.exists()) {
			try {
				rc.createNewFile();

			} catch (IOException e) {
				e.printStackTrace();
				return;
			}
		}

		try {
			br = new BufferedReader(new InputStreamReader(new FileInputStream(
					rc), "UTF8"));

			while ((buf = br.readLine()) != null) {
				parseLine(buf);

			}

			br.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void writeFile() {
		File rc = getRcFile();
		TreeSet ts;
		Iterator iter;
		String str;
		PrintWriter pw;

		ts = new TreeSet(map.keySet());
		iter = ts.iterator();

		try {
			pw = new PrintWriter(new OutputStreamWriter(
					new FileOutputStream(rc), "UTF8"));

			while (iter.hasNext()) {
				str = iter.next().toString();
				pw.println(str + "::" + map.get(str));

			}

			pw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	public boolean getBooleanValue(String key) {
		return getValue(key).equalsIgnoreCase("true");
	}

	public int getIntValue(String key) {
		return Integer.parseInt(getValue(key));
	}

	public String getStringValue(String key) {
		return getValue(key);
	}

	private String getValue(String key) {
		if (map.get(key) != null) {
			return (String) map.get(key);
		}
		return "";
	}

	public void setValue(String key, boolean value) {
		if (value) {
			setValue(key, "true");
		} else {
			setValue(key, "false");
		}
	}

	public void setValue(String key, int value) {
		setValue(key, Integer.toString(value));
	}

	public void setValue(String key, String value) {
		map.put(key, value);
	}

	public void removeValue(String key) {
		map.remove(key);
	}

	public Site getFavorite(String id) {
		Site fa;
		Vector f = getFavorites();
		Iterator iter = f.iterator();

		while (iter.hasNext()) {
			fa = (Site) iter.next();
			if (id.equalsIgnoreCase(fa.name) || id.equalsIgnoreCase(fa.alias)) {
				return fa;
			}
		}

		return null;
	}

	public Vector getFavorites() {
		Vector f = new Vector();
		int count = 0;
		String s;

		while (true) {
			s = getStringValue("favorite." + count);
			if (s.length() != 0) {
				f.addElement(new Site(s));
				count++;
			} else {
				break;
			}
		}

		return f;
	}

	public void setFavorites(Vector favorites) {
		Site f;
		Iterator iter = favorites.iterator();
		int count = 0;

		removeAllFavorites();

		while (iter.hasNext()) {
			f = (Site) iter.next();
			setValue("favorite." + count, f.toString());
			count++;
		}
	}

	public void removeAllFavorites() {
		String tmp;
		Iterator iter = map.keySet().iterator();

		while (iter.hasNext()) {
			tmp = iter.next().toString();
			if (tmp.startsWith("favorite.")) {
				iter.remove();
			}
		}

	}

}
