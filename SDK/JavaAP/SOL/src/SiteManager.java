import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.ButtonGroup;
import javax.swing.DefaultListModel;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

class ParameterPanel extends JPanel implements ActionListener {

	private JLabel nameLabel, hostLabel, portLabel, protocolLabel, aliasLabel;
	private JLabel userPromptLabel, userLabel, passPromptLabel, passLabel;
	private JLabel autoConnectLabel, autoLoginLabel;

	private JTextField nameField, hostField, portField, aliasField;
	private JTextField userPromptField, userField, passPromptField, passField;
	private JCheckBox autoConnectCheckBox, autoLoginCheckBox;
	private ButtonGroup protocolGroup;
	private JRadioButton sshButton, telnetButton;

	private JButton updateButton;

	private SiteManager parent;

	public void updateParameter(Site s) {
		nameField.setText(s.name);
		hostField.setText(s.host);
		portField.setText(Integer.toString(s.port));
		aliasField.setText(s.alias);

		if (s.protocol == Network.TELNET) {
			telnetButton.setSelected(true);
		} else if (s.protocol == Network.SSH) {
			sshButton.setSelected(true);
		}

		autoConnectCheckBox.setSelected(s.autoconnect);
	}

	public void actionPerformed(ActionEvent ae) {
		if (ae.getSource() == updateButton) {
			Site s = new Site(nameField.getText(), hostField.getText(), Integer
					.parseInt(portField.getText()));
			s.alias = aliasField.getText();

			if (telnetButton.isSelected()) {
				s.protocol = Network.TELNET;
			} else if (sshButton.isSelected()) {
				s.protocol = Network.SSH;
			}

			s.autoconnect = autoConnectCheckBox.isSelected();
			s.autologin = autoLoginCheckBox.isSelected();
			parent.updateFavorite(s);
		} else if (ae.getSource() == telnetButton) {
			portField.setText(Integer.toString(23));
		} else if (ae.getSource() == sshButton) {
			portField.setText(Integer.toString(22));
		}
	}

	public ParameterPanel(SiteManager p) {
		super();
		parent = p;
		setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();

		nameLabel = new JLabel("名稱");
		hostLabel = new JLabel("位置");
		portLabel = new JLabel("埠號");
		aliasLabel = new JLabel("別名");
		protocolLabel = new JLabel("通訊協定");

		userPromptLabel = new JLabel("帳號提示");
		userLabel = new JLabel("帳號");
		passPromptLabel = new JLabel("密碼提示");
		passLabel = new JLabel("密碼");
		autoConnectLabel = new JLabel("自動連線");
		autoLoginLabel = new JLabel("自動登入");

		nameField = new JTextField(15);
		hostField = new JTextField(15);
		portField = new JTextField(15);
		aliasField = new JTextField(15);

		telnetButton = new JRadioButton("Telnet");
		telnetButton.addActionListener(this);
		sshButton = new JRadioButton("SSH");
		sshButton.addActionListener(this);

		protocolGroup = new ButtonGroup();
		protocolGroup.add(telnetButton);
		protocolGroup.add(sshButton);

		userPromptField = new JTextField(15);
		userField = new JTextField(15);
		passPromptField = new JTextField(15);
		passField = new JTextField(15);
		autoConnectCheckBox = new JCheckBox();
		autoLoginCheckBox = new JCheckBox();

		updateButton = new JButton("更新");
		updateButton.addActionListener(this);

		c.gridx = 0;
		c.gridy = 0;
		c.gridwidth = 1;
		add(nameLabel, c);
		c.gridx = 1;
		c.gridwidth = 2;
		add(nameField, c);

		c.gridx = 0;
		c.gridy = 1;
		c.gridwidth = 1;
		add(hostLabel, c);
		c.gridx = 1;
		c.gridwidth = 2;
		add(hostField, c);

		c.gridx = 0;
		c.gridy = 2;
		c.gridwidth = 1;
		add(portLabel, c);
		c.gridx = 1;
		c.gridwidth = 2;
		add(portField, c);

		c.gridx = 0;
		c.gridy = 3;
		c.gridwidth = 1;
		add(aliasLabel, c);
		c.gridx = 1;
		c.gridwidth = 2;
		add(aliasField, c);

		c.gridx = 0;
		c.gridy = 4;
		c.gridwidth = 1;
		add(protocolLabel, c);
		c.gridx = 1;
		add(telnetButton, c);
		c.gridx = 2;
		add(sshButton, c);

		c.gridx = 0;
		c.gridy = 5;
		c.gridwidth = 1;
		add(autoConnectLabel, c);
		c.gridx = 1;
		add(autoConnectCheckBox, c);


		c.gridx = 0;
		c.gridy = 9;
		add(updateButton, c);
	}
}

class SitePanel extends JPanel implements ActionListener, ListSelectionListener {
	private JList siteList;
	private DefaultListModel siteListModel;

	private JPanel modifyPanel;
	private JButton addButton, removeButton, upButton, downButton;

	private SiteManager parent;
	private Vector favorites;

	private void makeList() {
		Iterator iter = favorites.iterator();
		siteListModel = new DefaultListModel();

		while (iter.hasNext()) {
			siteListModel.addElement(((Site) iter.next()).name);
		}

		siteList = new JList(siteListModel);
		siteList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		siteList.addListSelectionListener(this);
		siteList.setSelectedIndex(0);
	}

	private void makeModify() {
		modifyPanel = new JPanel();
		modifyPanel.setLayout(new GridLayout(0, 2, 3, 3));

		addButton = new JButton("新增");
		addButton.addActionListener(this);
		removeButton = new JButton("移除");
		removeButton.addActionListener(this);
		upButton = new JButton("上移");
		upButton.addActionListener(this);
		downButton = new JButton("下移");
		downButton.addActionListener(this);

		modifyPanel.add(addButton);
		modifyPanel.add(removeButton);
		modifyPanel.add(upButton);
		modifyPanel.add(downButton);
	}

	public SitePanel(SiteManager p, Vector f) {
		super();

		parent = p;
		favorites = f;

		makeList();
		makeModify();

		setLayout(new BorderLayout());
		add(new JScrollPane(siteList), BorderLayout.CENTER);
		add(modifyPanel, BorderLayout.SOUTH);
	}

	public void updateFavorite(Site f) {
		int index = siteList.getSelectedIndex();
		if (index != -1) {
			siteListModel.setElementAt(f.name, index);
			favorites.setElementAt(f, index);
		}
	}

	public void valueChanged(ListSelectionEvent lse) {
		int index = siteList.getSelectedIndex();
		if (index != -1) {
			parent.updateParameter((Site) favorites.elementAt(index));
		}
	}

	public void actionPerformed(ActionEvent ae) {
		if (ae.getSource() == addButton) {
			siteListModel.addElement("新站台");
			favorites.add(new Site("新站台", "hostname", 23));
			siteList.setSelectedIndex(siteListModel.getSize() - 1);
		} else if (ae.getSource() == removeButton) {
			int i = siteList.getSelectedIndex();
			if (i != -1) {
				siteListModel.removeElementAt(i);
				favorites.removeElementAt(i);
				siteList.setSelectedIndex(i - 1);
			}
		} else if (ae.getSource() == upButton) {
			int i = siteList.getSelectedIndex();
			if (i > 0) {
				Object tmp;

				tmp = favorites.elementAt(i);
				favorites.removeElementAt(i);
				favorites.insertElementAt(tmp, i - 1);

				tmp = siteListModel.elementAt(i);
				siteListModel.removeElementAt(i);
				siteListModel.insertElementAt(tmp, i - 1);

				siteList.setSelectedIndex(i - 1);
			}
		} else if (ae.getSource() == downButton) {
			int i = siteList.getSelectedIndex();
			if (i < siteListModel.size() - 1) {
				Object tmp;

				tmp = favorites.elementAt(i);
				favorites.removeElementAt(i);
				favorites.insertElementAt(tmp, i + 1);

				tmp = siteListModel.elementAt(i);
				siteListModel.removeElementAt(i);
				siteListModel.insertElementAt(tmp, i + 1);

				siteList.setSelectedIndex(i + 1);
			}
		}
	}
}

public class SiteManager extends JDialog implements ActionListener {

	private S_O_L parent;
	private Resource resource;
	private Vector favorites;

	private JSplitPane jsp;
	private ParameterPanel parameterPanel;
	private SitePanel sitePanel;

	private JButton okButton, cancelButton;
	private JPanel controlPanel;

	private void submit() {
		resource.setFavorites(favorites);
		resource.writeFile();

		parent.updateFavoriteMenu();
	}

	public void updateParameter(Site s) {
		parameterPanel.updateParameter(s);
	}

	public void updateFavorite(Site s) {
		sitePanel.updateFavorite(s);
	}

	public void actionPerformed(ActionEvent ae) {
		if (ae.getSource() == okButton) {
			submit();
			dispose();
		} else if (ae.getSource() == cancelButton) {
			dispose();
		}
	}

	public SiteManager(S_O_L pa, Resource r) {
		super(pa, "站台管理", false);

		parent = pa;
		resource = r;
		favorites = r.getFavorites();

		parameterPanel = new ParameterPanel(this);
		sitePanel = new SitePanel(this, favorites);

		jsp = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, sitePanel,
				parameterPanel);
		getContentPane().add(jsp, BorderLayout.CENTER);

		okButton = new JButton("確定");
		okButton.addActionListener(this);

		cancelButton = new JButton("取消");
		cancelButton.addActionListener(this);

		controlPanel = new JPanel();
		controlPanel.setLayout(new FlowLayout());
		controlPanel.add(okButton);
		controlPanel.add(cancelButton);
		getContentPane().add(controlPanel, BorderLayout.SOUTH);

		jsp.setDividerLocation(160);

		setSize(500, 375);
		setLocationRelativeTo(null);
		setVisible(true);
	}
}
