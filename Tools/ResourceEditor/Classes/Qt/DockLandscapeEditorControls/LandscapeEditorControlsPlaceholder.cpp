#include "LandscapeEditorControlsPlaceholder.h"
#include "../Scene/SceneSignals.h"
#include "../Scene/SceneEditor2.h"

#include "CustomColorsPanel.h"
#include "RulerToolPanel.h"
#include "VisibilityToolPanel.h"
#include "TilemaskEditorPanel.h"
#include "HeightmapEditorPanel.h"

#include <QVBoxLayout>

LandscapeEditorControlsPlaceholder::LandscapeEditorControlsPlaceholder(QWidget* parent)
:	QWidget(parent)
,	currentPanel(NULL)
,	activeScene(NULL)
{
	InitUI();
	ConnectToSignals();

	customColorsPanel = new CustomColorsPanel();
	rulerToolPanel = new RulerToolPanel();
	visibilityToolPanel = new VisibilityToolPanel();
	tilemaskEditorPanel = new TilemaskEditorPanel();
	heightmapEditorPanel = new HeightmapEditorPanel();
}

LandscapeEditorControlsPlaceholder::~LandscapeEditorControlsPlaceholder()
{
	SafeDelete(customColorsPanel);
	SafeDelete(rulerToolPanel);
	SafeDelete(visibilityToolPanel);
	SafeDelete(tilemaskEditorPanel);
	SafeDelete(heightmapEditorPanel);
}

void LandscapeEditorControlsPlaceholder::InitUI()
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	
	setLayout(layout);
}

void LandscapeEditorControlsPlaceholder::ConnectToSignals()
{
	connect(SceneSignals::Instance(), SIGNAL(Activated(SceneEditor2*)), this, SLOT(SceneActivated(SceneEditor2*)));
	connect(SceneSignals::Instance(), SIGNAL(Deactivated(SceneEditor2*)), this, SLOT(SceneDeactivated(SceneEditor2*)));

	connect(SceneSignals::Instance(), SIGNAL(VisibilityToolToggled(SceneEditor2*)),
			this, SLOT(EditorToggled(SceneEditor2*)));
	connect(SceneSignals::Instance(), SIGNAL(CustomColorsToggled(SceneEditor2*)),
			this, SLOT(EditorToggled(SceneEditor2*)));
	connect(SceneSignals::Instance(), SIGNAL(HeightmapEditorToggled(SceneEditor2*)),
			this, SLOT(EditorToggled(SceneEditor2*)));
	connect(SceneSignals::Instance(), SIGNAL(TilemaskEditorToggled(SceneEditor2*)),
			this, SLOT(EditorToggled(SceneEditor2*)));
	connect(SceneSignals::Instance(), SIGNAL(RulerToolToggled(SceneEditor2*)),
			this, SLOT(EditorToggled(SceneEditor2*)));
}

void LandscapeEditorControlsPlaceholder::SceneActivated(SceneEditor2* scene)
{
	activeScene = scene;

	UpdatePanels();
}

void LandscapeEditorControlsPlaceholder::SceneDeactivated(SceneEditor2* scene)
{
	RemovePanel();

	activeScene = NULL;
}

void LandscapeEditorControlsPlaceholder::SetPanel(LandscapeEditorBasePanel* panel)
{
	if (!panel || panel == currentPanel)
	{
		return;
	}
	
	RemovePanel();
	
	currentPanel = panel;
	layout()->addWidget(panel);

	panel->InitPanel(activeScene);
}

void LandscapeEditorControlsPlaceholder::RemovePanel()
{
	if (!currentPanel)
	{
		return;
	}
	
	currentPanel->DeinitPanel();
	
	currentPanel->setParent(NULL);
	layout()->removeWidget(currentPanel);
	currentPanel = NULL;
}

void LandscapeEditorControlsPlaceholder::EditorToggled(SceneEditor2* scene)
{
	if (scene != activeScene)
	{
		return;
	}

	UpdatePanels();
}

void LandscapeEditorControlsPlaceholder::UpdatePanels()
{
	RemovePanel();

	int32 tools = activeScene->GetEnabledTools();
	if (tools & SceneEditor2::LANDSCAPE_TOOL_CUSTOM_COLOR)
	{
		SetPanel(customColorsPanel);
	}
	else if (tools & SceneEditor2::LANDSCAPE_TOOL_HEIGHTMAP_EDITOR)
	{
		SetPanel(heightmapEditorPanel);
	}
	else if (tools & SceneEditor2::LANDSCAPE_TOOL_RULER)
	{
		SetPanel(rulerToolPanel);
	}
	else if (tools & SceneEditor2::LANDSCAPE_TOOL_TILEMAP_EDITOR)
	{
		SetPanel(tilemaskEditorPanel);
	}
	else if (tools & SceneEditor2::LANDSCAPE_TOOL_VISIBILITY)
	{
		SetPanel(visibilityToolPanel);
	}
}
