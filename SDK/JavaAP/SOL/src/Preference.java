import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.JSplitPane;
import javax.swing.JTextField;
import javax.swing.JTree;
import javax.swing.SpinnerNumberModel;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeSelectionModel;

class GeneralPanel extends JPanel implements ActionListener {
	private Resource resource;

	public JLabel browserLabel;
	public JTextField browserField;

	public JLabel copyOnSelectLabel, clearAfterCopyLabel, removeManualLabel,
			linebreakLabel;
	public JCheckBox copyOnSelectCheckBox, clearAfterCopyCheckBox,
			removeManualCheckBox, linebreakCheckBox;

	public JLabel breaklengthLabel;
	public JSpinner breaklengthSpinner;
	public SpinnerNumberModel breaklengthModel;

	public JLabel customBellLabel;
	public JCheckBox customBellCheckBox;

	public JLabel bellPathLabel;
	public JTextField bellPathField;
	public JButton bellPathButton;

	public void actionPerformed(ActionEvent ae) {
		if (ae.getSource() == linebreakCheckBox) {
			breaklengthSpinner.setEnabled(linebreakCheckBox.isSelected());
		} else if (ae.getSource() == customBellCheckBox) {
			bellPathField.setEnabled(customBellCheckBox.isSelected());
			bellPathButton.setEnabled(customBellCheckBox.isSelected());
		} else if (ae.getSource() == bellPathButton) {
			JFileChooser jfc = new JFileChooser();
			if (jfc.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
				bellPathField.setText(jfc.getSelectedFile().getAbsolutePath());
			}
		}
	}

	public GeneralPanel(Resource r) {
		super();
		resource = r;

		browserLabel = new JLabel("外部瀏覽器啟動命令（%u 將替換為網址）：");
		browserField = new JTextField(resource
				.getStringValue(Resource.EXTERNAL_BROWSER), 20);

		copyOnSelectLabel = new JLabel("選取後立即複製");
		copyOnSelectCheckBox = new JCheckBox();
		copyOnSelectCheckBox.setSelected(resource
				.getBooleanValue(Resource.COPY_ON_SELECT));

		clearAfterCopyLabel = new JLabel("複製後取消選取");
		clearAfterCopyCheckBox = new JCheckBox();
		clearAfterCopyCheckBox.setSelected(resource
				.getBooleanValue(Resource.CLEAR_AFTER_COPY));

		removeManualLabel = new JLabel("手動斷線時移除分頁");
		removeManualCheckBox = new JCheckBox();
		removeManualCheckBox.setSelected(resource
				.getBooleanValue(Resource.REMOVE_MANUAL_DISCONNECT));

		linebreakLabel = new JLabel("貼上文字時自動換行");
		linebreakCheckBox = new JCheckBox();
		linebreakCheckBox.setSelected(resource
				.getBooleanValue(Resource.AUTO_LINE_BREAK));
		linebreakCheckBox.addActionListener(this);

		breaklengthLabel = new JLabel("每行最多幾個字");
		breaklengthModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.AUTO_LINE_BREAK_LENGTH), 1, 512, 1);
		breaklengthSpinner = new JSpinner(breaklengthModel);
		breaklengthSpinner.setEnabled(resource
				.getBooleanValue(Resource.AUTO_LINE_BREAK));

		customBellLabel = new JLabel("使用自訂的音效");
		customBellCheckBox = new JCheckBox();
		customBellCheckBox.setSelected(resource
				.getBooleanValue(Resource.USE_CUSTOM_BELL));
		customBellCheckBox.addActionListener(this);

		bellPathLabel = new JLabel("音效檔路徑");
		bellPathField = new JTextField(resource
				.getStringValue(Resource.CUSTOM_BELL_PATH), 15);
		bellPathField.setEnabled(resource
				.getBooleanValue(Resource.USE_CUSTOM_BELL));
		bellPathButton = new JButton("瀏覽");
		bellPathButton.setEnabled(resource
				.getBooleanValue(Resource.USE_CUSTOM_BELL));
		bellPathButton.addActionListener(this);

		setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.anchor = GridBagConstraints.WEST;

		c.gridx = 0;
		c.gridy = 0;
		c.gridwidth = 3;
		add(browserLabel, c);

		c.gridx = 0;
		c.gridy = 1;
		c.gridwidth = 3;
		add(browserField, c);

		c.gridwidth = 1;

		c.gridx = 0;
		c.gridy = 2;
		add(copyOnSelectLabel, c);
		c.gridx = 1;
		add(copyOnSelectCheckBox, c);

		c.gridx = 0;
		c.gridy = 3;
		add(clearAfterCopyLabel, c);
		c.gridx = 1;
		add(clearAfterCopyCheckBox, c);

		c.gridx = 0;
		c.gridy = 4;
		add(removeManualLabel, c);
		c.gridx = 1;
		add(removeManualCheckBox, c);

		c.gridx = 0;
		c.gridy = 5;
		add(linebreakLabel, c);
		c.gridx = 1;
		add(linebreakCheckBox, c);

		c.gridx = 0;
		c.gridy = 6;
		add(breaklengthLabel, c);
		c.gridx = 1;
		add(breaklengthSpinner, c);

		c.gridx = 0;
		c.gridy = 7;
		add(customBellLabel, c);
		c.gridx = 1;
		add(customBellCheckBox, c);

		c.gridx = 0;
		c.gridy = 8;
		add(bellPathLabel, c);
		c.gridx = 1;
		add(bellPathField, c);
		c.gridx = 2;
		add(bellPathButton, c);
	}
}

class ConnectionPanel extends JPanel implements ActionListener {
	private Resource resource;

	public JLabel autoReconnectLabel, reconnectTimeLabel,
			reconnectIntervalLabel;
	public JCheckBox autoReconnectCheckBox;
	public JSpinner reconnectTimeSpinner, reconnectIntervalSpinner;
	public SpinnerNumberModel reconnectTimeModel, reconnectIntervalModel;

	public JLabel antiIdleLabel, antiIdleTimeLabel;
	public JCheckBox antiIdleCheckBox;
	public JSpinner antiIdleTimeSpinner;
	public SpinnerNumberModel antiIdleModel;

	public void actionPerformed(ActionEvent ae) {
		if (ae.getSource() == autoReconnectCheckBox) {
			reconnectTimeSpinner.setEnabled(autoReconnectCheckBox.isSelected());
			reconnectIntervalSpinner.setEnabled(autoReconnectCheckBox
					.isSelected());
		} else if (ae.getSource() == antiIdleCheckBox) {
			antiIdleTimeSpinner.setEnabled(antiIdleCheckBox.isSelected());
		}
	}

	public ConnectionPanel(Resource r) {
		super();
		resource = r;

		boolean autoReconnect = resource
				.getBooleanValue(Resource.AUTO_RECONNECT);
		autoReconnectLabel = new JLabel("斷線自動重連");
		autoReconnectCheckBox = new JCheckBox();
		autoReconnectCheckBox.setSelected(autoReconnect);
		autoReconnectCheckBox.addActionListener(this);

		reconnectTimeLabel = new JLabel("多久時間內重連（秒）");
		reconnectTimeModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.AUTO_RECONNECT_TIME), 0, 3600, 1);
		reconnectTimeSpinner = new JSpinner(reconnectTimeModel);
		reconnectTimeSpinner.setEnabled(autoReconnect);

		reconnectIntervalLabel = new JLabel("重連前等待時間（毫秒）");
		reconnectIntervalModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.AUTO_RECONNECT_INTERVAL), 0, 60000, 1);
		reconnectIntervalSpinner = new JSpinner(reconnectIntervalModel);
		reconnectIntervalSpinner.setEnabled(autoReconnect);

		antiIdleLabel = new JLabel("自動防閒置");
		antiIdleCheckBox = new JCheckBox();
		antiIdleCheckBox.setSelected(resource
				.getBooleanValue(Resource.ANTI_IDLE));
		antiIdleCheckBox.addActionListener(this);

		antiIdleTimeLabel = new JLabel("防閒置時間（秒）");
		antiIdleModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.ANTI_IDLE_INTERVAL), 0, 3600, 1);
		antiIdleTimeSpinner = new JSpinner(antiIdleModel);
		antiIdleTimeSpinner.setEnabled(resource
				.getBooleanValue(Resource.ANTI_IDLE));

		setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.anchor = GridBagConstraints.WEST;

		c.gridx = 0;
		c.gridy = 0;
		add(autoReconnectLabel, c);
		c.gridx = 1;
		add(autoReconnectCheckBox, c);

		c.gridx = 0;
		c.gridy = 1;
		add(reconnectTimeLabel, c);
		c.gridx = 1;
		add(reconnectTimeSpinner, c);

		c.gridx = 0;
		c.gridy = 2;
		add(reconnectIntervalLabel, c);
		c.gridx = 1;
		add(reconnectIntervalSpinner, c);

		c.gridx = 0;
		c.gridy = 3;
		add(antiIdleLabel, c);
		c.gridx = 1;
		add(antiIdleCheckBox, c);

		c.gridx = 0;
		c.gridy = 4;
		add(antiIdleTimeLabel, c);
		c.gridx = 1;
		add(antiIdleTimeSpinner, c);
	}
}

class ApperancePanel extends JPanel {
	private Resource resource;

	public JLabel systemLookFeelLabel, showToolbarLabel;
	public JCheckBox systemLookFeelCheckBox, showToolbarCheckBox;

	public JLabel widthLabel, heightLabel;
	public JSpinner widthSpinner, heightSpinner;
	public SpinnerNumberModel widthModel, heightModel;

	public JLabel terminalRowsLabel, terminalColumnsLabel;
	public JSpinner terminalRowsSpinner, terminalColumnsSpinner;
	public SpinnerNumberModel terminalRowsModel, terminalColumnsModel;

	public ApperancePanel(Resource r) {
		super();
		resource = r;

		systemLookFeelLabel = new JLabel("使用系統 Look and Feel");
		systemLookFeelCheckBox = new JCheckBox();
		systemLookFeelCheckBox.setSelected(resource
				.getBooleanValue(Resource.SYSTEM_LOOK_FEEL));

		showToolbarLabel = new JLabel("是否顯示工具列");
		showToolbarCheckBox = new JCheckBox();
		showToolbarCheckBox.setSelected(resource
				.getBooleanValue(Resource.SHOW_TOOLBAR));

		widthLabel = new JLabel("視窗寬度");
		widthModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.GEOMETRY_WIDTH), 0, 4096, 1);
		widthSpinner = new JSpinner(widthModel);

		heightLabel = new JLabel("視窗長度");
		heightModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.GEOMETRY_HEIGHT), 0, 4096, 1);
		heightSpinner = new JSpinner(heightModel);

		terminalColumnsLabel = new JLabel("模擬終端機行數");
		terminalColumnsModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.TERMINAL_COLUMNS), 80, 200, 1);
		terminalColumnsSpinner = new JSpinner(terminalColumnsModel);
		terminalColumnsSpinner.setEnabled(false);

		terminalRowsLabel = new JLabel("模擬終端機列數");
		terminalRowsModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.TERMINAL_ROWS), 24, 200, 1);
		terminalRowsSpinner = new JSpinner(terminalRowsModel);
		terminalRowsSpinner.setEnabled(false);

		setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.anchor = GridBagConstraints.WEST;

		c.gridx = 0;
		c.gridy = 0;
		add(systemLookFeelLabel, c);
		c.gridx = 1;
		add(systemLookFeelCheckBox, c);

		c.gridx = 0;
		c.gridy = 1;
		add(showToolbarLabel, c);
		c.gridx = 1;
		add(showToolbarCheckBox, c);

		c.gridx = 0;
		c.gridy = 2;
		add(widthLabel, c);
		c.gridx = 1;
		add(widthSpinner, c);

		c.gridx = 0;
		c.gridy = 3;
		add(heightLabel, c);
		c.gridx = 1;
		add(heightSpinner, c);

		c.gridx = 0;
		c.gridy = 4;
		add(terminalColumnsLabel, c);
		c.gridx = 1;
		add(terminalColumnsSpinner, c);

		c.gridx = 0;
		c.gridy = 5;
		add(terminalRowsLabel, c);
		c.gridx = 1;
		add(terminalRowsSpinner, c);

	}
}

class FontPanel extends JPanel implements ActionListener {
	private Resource resource;

	public JLabel useCustomFontLabel;
	public JCheckBox useCustomFontCheckBox;

	public JLabel fontSizeLabel;
	public JSpinner fontSizeSpinner;
	public SpinnerNumberModel fontSizeModel;

	public JLabel ttfPathLabel;
	public JTextField ttfPathField;
	public JButton ttfPathButton;

	public JLabel fontVerticalGapLabel, fontHorizontalGapLabel,
			fontDescentAdjustLabel;
	public JSpinner fontVerticalGapSpinner, fontHorizontalGapSpinner,
			fontDescentAdjustSpinner;
	public SpinnerNumberModel fontVerticalGapModel, fontHorizontalGapModel,
			fontDescentAdjustModel;

	public void actionPerformed(ActionEvent e) {
		if (e.getSource() == useCustomFontCheckBox) {
			ttfPathButton.setEnabled(useCustomFontCheckBox.isSelected());
			ttfPathField.setEnabled(useCustomFontCheckBox.isSelected());
		} else if (e.getSource() == ttfPathButton) {
			JFileChooser jfc = new JFileChooser();
			if (jfc.showOpenDialog(this) == JFileChooser.APPROVE_OPTION) {
				ttfPathField.setText(jfc.getSelectedFile().getAbsolutePath());
			}
		}
	}

	public FontPanel(Resource r) {
		super();
		resource = r;

		fontSizeLabel = new JLabel("字體大小（0 則自動配置）");
		fontSizeModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.FONT_SIZE), 0, 64, 1);
		fontSizeSpinner = new JSpinner(fontSizeModel);

		useCustomFontLabel = new JLabel("使用自訂字型");
		useCustomFontCheckBox = new JCheckBox();
		useCustomFontCheckBox.setSelected(resource
				.getBooleanValue(Resource.FONT_USE_CUSTOM_FONT));
		useCustomFontCheckBox.addActionListener(this);

		ttfPathLabel = new JLabel("字型路徑（TTF）");
		ttfPathField = new JTextField(resource
				.getStringValue(Resource.FONT_TTF_PATH), 15);
		ttfPathField.setEnabled(resource
				.getBooleanValue(Resource.FONT_USE_CUSTOM_FONT));

		ttfPathButton = new JButton("瀏覽");
		ttfPathButton.setEnabled(resource
				.getBooleanValue(Resource.FONT_USE_CUSTOM_FONT));
		ttfPathButton.addActionListener(this);

		fontVerticalGapLabel = new JLabel("字體垂直間距");
		fontVerticalGapModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.FONT_VERTICLAL_GAP), -10, 10, 1);
		fontVerticalGapSpinner = new JSpinner(fontVerticalGapModel);

		fontHorizontalGapLabel = new JLabel("字體水平間距");
		fontHorizontalGapModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.FONT_HORIZONTAL_GAP), -10, 10, 1);
		fontHorizontalGapSpinner = new JSpinner(fontHorizontalGapModel);

		fontDescentAdjustLabel = new JLabel("字體 Descent 微調");
		fontDescentAdjustModel = new SpinnerNumberModel(resource
				.getIntValue(Resource.FONT_DESCENT_ADJUST), -10, 10, 1);
		fontDescentAdjustSpinner = new JSpinner(fontDescentAdjustModel);

		setLayout(new GridBagLayout());
		GridBagConstraints c = new GridBagConstraints();
		c.anchor = GridBagConstraints.WEST;

		c.gridx = 0;
		c.gridy = 0;
		add(fontSizeLabel, c);
		c.gridx = 1;
		add(fontSizeSpinner, c);

		c.gridx = 0;
		c.gridy = 1;
		add(useCustomFontLabel, c);
		c.gridx = 1;
		add(useCustomFontCheckBox, c);

		c.gridx = 0;
		c.gridy = 2;
		add(ttfPathLabel, c);
		c.gridx = 1;
		add(ttfPathField, c);
		c.gridx = 2;
		add(ttfPathButton, c);

		c.gridx = 0;
		c.gridy = 3;
		add(fontVerticalGapLabel, c);
		c.gridx = 1;
		add(fontVerticalGapSpinner, c);

		c.gridx = 0;
		c.gridy = 4;
		add(fontHorizontalGapLabel, c);
		c.gridx = 1;
		add(fontHorizontalGapSpinner, c);

		c.gridx = 0;
		c.gridy = 5;
		add(fontDescentAdjustLabel, c);
		c.gridx = 1;
		add(fontDescentAdjustSpinner, c);
	}

}

public class Preference extends JDialog implements ActionListener,
		TreeSelectionListener {

	private S_O_L parent;
	private Resource resource;

	private JSplitPane jsp;
	private GeneralPanel gp;
	private ConnectionPanel cp;
	private ApperancePanel ap;
	private FontPanel fp;
	private JPanel welcome;

	private JTree categoryTree;
	private DefaultMutableTreeNode rootNode, generalNode, connectionNode,
			appearanceNode, fontNode;

	private JButton okButton, cancelButton, applyButton;
	private JPanel controlPanel;

	public void submit() {
		resource.setValue(Resource.EXTERNAL_BROWSER, gp.browserField.getText());
		resource.setValue(Resource.COPY_ON_SELECT, gp.copyOnSelectCheckBox
				.isSelected());
		resource.setValue(Resource.CLEAR_AFTER_COPY, gp.clearAfterCopyCheckBox
				.isSelected());
		resource.setValue(Resource.REMOVE_MANUAL_DISCONNECT,
				gp.removeManualCheckBox.isSelected());
		resource.setValue(Resource.AUTO_LINE_BREAK, gp.linebreakCheckBox
				.isSelected());
		resource.setValue(Resource.AUTO_LINE_BREAK_LENGTH, gp.breaklengthModel
				.getValue().toString());
		resource.setValue(Resource.USE_CUSTOM_BELL, gp.customBellCheckBox
				.isSelected());
		resource
				.setValue(Resource.CUSTOM_BELL_PATH, gp.bellPathField.getText());

		resource.setValue(Resource.AUTO_RECONNECT, cp.autoReconnectCheckBox
				.isSelected());
		resource.setValue(Resource.AUTO_RECONNECT_TIME, cp.reconnectTimeModel
				.getValue().toString());
		resource.setValue(Resource.AUTO_RECONNECT_INTERVAL,
				cp.reconnectIntervalModel.getValue().toString());

		resource.setValue(Resource.ANTI_IDLE, cp.antiIdleCheckBox.isSelected());
		resource.setValue(Resource.ANTI_IDLE_INTERVAL, cp.antiIdleModel
				.getValue().toString());

		resource.setValue(Resource.SYSTEM_LOOK_FEEL, ap.systemLookFeelCheckBox
				.isSelected());
		resource.setValue(Resource.SHOW_TOOLBAR, ap.showToolbarCheckBox
				.isSelected());
		resource.setValue(Resource.GEOMETRY_WIDTH, ap.widthModel.getValue()
				.toString());
		resource.setValue(Resource.GEOMETRY_HEIGHT, ap.heightModel.getValue()
				.toString());
		resource.setValue(Resource.TERMINAL_COLUMNS, ap.terminalColumnsModel
				.getValue().toString());
		resource.setValue(Resource.TERMINAL_ROWS, ap.terminalRowsModel
				.getValue().toString());

		resource.setValue(Resource.FONT_USE_CUSTOM_FONT,
				fp.useCustomFontCheckBox.isSelected());
		resource.setValue(Resource.FONT_SIZE, fp.fontSizeModel.getValue()
				.toString());
		resource.setValue(Resource.FONT_TTF_PATH, fp.ttfPathField.getText());
		resource.setValue(Resource.FONT_VERTICLAL_GAP, fp.fontVerticalGapModel
				.getValue().toString());
		resource.setValue(Resource.FONT_HORIZONTAL_GAP,
				fp.fontHorizontalGapModel.getValue().toString());
		resource.setValue(Resource.FONT_DESCENT_ADJUST,
				fp.fontDescentAdjustModel.getValue().toString());

		resource.writeFile();

		parent.updateBounds();
		parent.updateLookAndFeel();
		parent.updateToolbar();
		parent.updateSize();
		parent.updateAntiIdleTime();
	}

	private void makeCategoryTree() {
		rootNode = new DefaultMutableTreeNode("設定");
		generalNode = new DefaultMutableTreeNode("一般");
		connectionNode = new DefaultMutableTreeNode("連線");
		appearanceNode = new DefaultMutableTreeNode("外觀");
		fontNode = new DefaultMutableTreeNode("字型");

		rootNode.add(generalNode);
		rootNode.add(connectionNode);
		rootNode.add(appearanceNode);
		rootNode.add(fontNode);

		categoryTree = new JTree(rootNode);
		categoryTree.getSelectionModel().setSelectionMode(
				TreeSelectionModel.SINGLE_TREE_SELECTION);
		categoryTree.addTreeSelectionListener(this);
	}

	public void actionPerformed(ActionEvent ae) {
		if (ae.getSource() == okButton) {
			submit();
			dispose();
		} else if (ae.getSource() == cancelButton) {
			dispose();
		} else if (ae.getSource() == applyButton) {
			submit();
		}
	}

	public void valueChanged(TreeSelectionEvent tse) {

		DefaultMutableTreeNode node = (DefaultMutableTreeNode) categoryTree
				.getLastSelectedPathComponent();

		if (node == null)
			return;

		if (node == generalNode) {
			jsp.setRightComponent(gp);
		} else if (node == connectionNode) {
			jsp.setRightComponent(cp);
		} else if (node == appearanceNode) {
			jsp.setRightComponent(ap);
		} else if (node == fontNode) {
			jsp.setRightComponent(fp);
		} else {
			jsp.setRightComponent(welcome);
		}
		jsp.setDividerLocation(150);
	}

	public Preference(S_O_L pa, Resource s) {
		super(pa, "偏好設定", false);

		parent = pa;
		resource = s;

		Rectangle r = parent.getBounds();
		resource.setValue(Resource.GEOMETRY_X, (int) r.getX());
		resource.setValue(Resource.GEOMETRY_Y, (int) r.getY());
		resource.setValue(Resource.GEOMETRY_WIDTH, (int) r.getWidth());
		resource.setValue(Resource.GEOMETRY_HEIGHT, (int) r.getHeight());

		makeCategoryTree();

		welcome = new JPanel();
		welcome.add(new JLabel("偏好設定"));

		jsp = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, categoryTree, welcome);
		jsp.setOneTouchExpandable(true);
		jsp.setDividerLocation(150);
		getContentPane().add(jsp, BorderLayout.CENTER);

		gp = new GeneralPanel(resource);
		cp = new ConnectionPanel(resource);
		ap = new ApperancePanel(resource);
		fp = new FontPanel(resource);

		okButton = new JButton("確定");
		okButton.addActionListener(this);

		cancelButton = new JButton("取消");
		cancelButton.addActionListener(this);

		applyButton = new JButton("套用");
		applyButton.addActionListener(this);

		controlPanel = new JPanel();
		controlPanel.setLayout(new FlowLayout());
		controlPanel.add(okButton);
		controlPanel.add(cancelButton);
		controlPanel.add(applyButton);

		getContentPane().add(controlPanel, BorderLayout.SOUTH);

		setSize(600, 450);
		setLocationRelativeTo(null);
		setVisible(true);
	}
}
