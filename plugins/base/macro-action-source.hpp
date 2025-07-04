#pragma once
#include "macro-action-edit.hpp"
#include "variable-text-edit.hpp"
#include "source-properties-button.hpp"
#include "source-selection.hpp"
#include "source-setting.hpp"

#include <QLabel>
#include <QPushButton>
#include <QComboBox>

namespace advss {

class MacroActionSource : public MacroAction {
public:
	MacroActionSource(Macro *m) : MacroAction(m) {}
	bool PerformAction();
	void LogAction() const;
	bool Save(obs_data_t *obj) const;
	bool Load(obs_data_t *obj);
	std::string GetShortDesc() const;
	std::string GetId() const { return id; };
	static std::shared_ptr<MacroAction> Create(Macro *m);
	std::shared_ptr<MacroAction> Copy() const;
	void ResolveVariablesToFixedValues();

	SourceSelection _source;
	SourceSettingButton _button;
	StringVariable _settingsString = "";
	StringVariable _manualSettingValue = "";
	obs_deinterlace_mode _deinterlaceMode = OBS_DEINTERLACE_MODE_DISABLE;
	obs_deinterlace_field_order _deinterlaceOrder =
		OBS_DEINTERLACE_FIELD_ORDER_TOP;
	TempVariableRef _tempVar;
	SourceSetting _setting;

	enum class Action {
		ENABLE,
		DISABLE,
		SETTINGS,
		REFRESH_SETTINGS,
		SETTINGS_BUTTON,
		DEINTERLACE_MODE,
		DEINTERLACE_FIELD_ORDER,
		OPEN_INTERACTION_DIALOG,
		OPEN_FILTER_DIALOG,
		OPEN_PROPERTIES_DIALOG,
	};
	Action _action = Action::SETTINGS;

	enum class SettingsInputMethod {
		INDIVIDUAL_MANUAL,
		INDIVIDUAL_TEMPVAR,
		JSON_STRING,
		INDIVIDUAL_LIST_ENTRY,
	};
	SettingsInputMethod _settingsInputMethod =
		SettingsInputMethod::INDIVIDUAL_MANUAL;

private:
	static bool _registered;
	static const std::string id;
};

class MacroActionSourceEdit : public QWidget {
	Q_OBJECT

public:
	MacroActionSourceEdit(
		QWidget *parent,
		std::shared_ptr<MacroActionSource> entryData = nullptr);
	void UpdateEntryData();
	static QWidget *Create(QWidget *parent,
			       std::shared_ptr<MacroAction> action)
	{
		return new MacroActionSourceEdit(
			parent,
			std::dynamic_pointer_cast<MacroActionSource>(action));
	}

private slots:
	void SourceChanged(const SourceSelection &);
	void ActionChanged(int value);
	void ButtonChanged(const SourceSettingButton &);
	void GetSettingsClicked();
	void SettingsStringChanged();
	void DeinterlaceModeChanged(int);
	void DeinterlaceOrderChanged(int);
	void SelectionChanged(const TempVariableRef &);
	void SettingsInputMethodChanged(int);
	void SelectionChanged(const SourceSetting &);
	void ManualSettingsValueChanged();
	void RefreshVariableSourceSelectionValue();

signals:
	void HeaderInfoChanged(const QString &);

private:
	void SetWidgetVisibility();

	SourceSelectionWidget *_sources;
	QComboBox *_actions;
	SourceSettingsButtonSelection *_settingsButtons;
	QHBoxLayout *_settingsLayout;
	QComboBox *_settingsInputMethods;
	VariableTextEdit *_manualSettingValue;
	TempVariableSelection *_tempVars;
	SourceSettingSelection *_sourceSettings;
	VariableTextEdit *_settingsString;
	QPushButton *_getSettings;
	QComboBox *_deinterlaceMode;
	QComboBox *_deinterlaceOrder;
	QLabel *_warning;
	QPushButton *_refreshSettingSelection;

	std::shared_ptr<MacroActionSource> _entryData;
	bool _loading = true;
};

} // namespace advss
