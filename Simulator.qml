import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
ApplicationWindow 
{ 
    id: window
    width: 1200
    height: 780
    visible: true 
    title: "Vole Machine Simulator — FCAI Edition" 
    color: "#181825"
    QtObject 
    {
        id: theme
        property color bgCard: "#1e1e2e"
        property color bgInput: "#313244"
        property color border: "#45475a"
        property color accentBlue: "#89b4fa"
        property color accentGreen: "#a6e3a1"
        property color accentRed: "#f38ba8"
        property color accentYellow: "#f9e2af"
        property color textPrimary: "#cdd6f4"
        property color textSecondary: "#a6adc8"
    }

    component StyledButton: Button
    {
        id: ctl
        property color accent: "#89b4fa"
        font.bold: true
        leftPadding: 14
        rightPadding: 14

        contentItem: Text
        {
            text: ctl.text
            font: ctl.font
            color: ctl.enabled ? "#cdd6f4" : "#6c7086"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle
        {
            implicitHeight: 34
            implicitWidth: 100
            radius: 6
            color: !ctl.enabled ? "#1e1e2e"
                   : ctl.down ? Qt.darker(ctl.accent, 1.6)
                   : ctl.hovered ? "#45475a" : "#313244"
            border.color: ctl.enabled ? ctl.accent : "#45475a"
            border.width: 1
        }

        MouseArea
        {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            cursorShape: ctl.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
        }
    }

    FileDialog 
    {
        id: fileDialog
        title: "Select Vole Program File"
        nameFilters: ["Text Files (*.txt)", "All Files (*)"]
        onAccepted: {
            var addr = parseInt(startAddrInput.text, 16)
            if (isNaN(addr)) addr = 0
            vm.loadProgram(fileDialog.selectedFile, addr)
        }
    }

    // نافذة حوارية لتعديل الذاكرة والمسجلات (Overwrite Feature)
    Dialog
    {
        id: editDialog
        parent: Overlay.overlay
        anchors.centerIn: parent
        modal: true
        focus: true
       

        property bool isRegister: true
        property int targetIndex: 0

        background: Rectangle
        {
            color: theme.bgCard
            radius: 8
            border.color: theme.accentBlue
        }

        header: Label
        {
            text: editDialog.isRegister
                  ? "Edit Register R" + editDialog.targetIndex.toString(16).toUpperCase()
                  : "Edit Memory [0x" + editDialog.targetIndex.toString(16).toUpperCase().padStart(2, '0') + "]"
            color: theme.accentBlue
            font.bold: true
            padding: 12
        }
        
        footer: Item
        {
            implicitHeight: 56

            RowLayout
            {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8

                Item { Layout.fillWidth: true }

                StyledButton
                {
                    text: "Cancel"
                    accent: "#f38ba8"
                    onClicked: editDialog.reject()
                }

                StyledButton
                {
                    text: "OK"
                    accent: "#a6e3a1"
                    onClicked: editDialog.accept()
                }
            }
        }

        ColumnLayout
        {
            spacing: 10
            Label { text: "New value (Hex, 00 - FF):"; color: theme.textPrimary }
            TextField
            {
                id: editInput
                text: "00"
                maximumLength: 2
                color: theme.textPrimary
                font.family: "Monospace"
                font.bold: true
                Layout.fillWidth: true
                validator: RegularExpressionValidator { regularExpression: /[0-9A-Fa-f]{0,2}/ }
                background: Rectangle { color: theme.bgInput; radius: 4; border.color: theme.border }
                onAccepted: editDialog.accept()
            }
        }

        onOpened: { editInput.forceActiveFocus(); editInput.selectAll() }

        onAccepted:
        {
            if (isRegister)
                vm.setRegister(targetIndex, editInput.text)
            else
                vm.setMemory(targetIndex, editInput.text)
        }
    }

    ColumnLayout 
    {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 10

        // ==========================================
        // 1. Top Controls Bar
        // ==========================================
        Rectangle 
        {
            Layout.fillWidth: true
            implicitHeight: 55
            color: theme.bgCard
            radius: 8
            border.color: theme.border

            RowLayout 
            {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                StyledButton
                {
                    text: "📁 Load Program"
                    accent: "#89b4fa"
                    onClicked: fileDialog.open()
                }

                Label { text: "Start Addr (Hex):"; color: theme.textSecondary }
                TextField 
                {
                    id: startAddrInput
                    color: "#cdd6f4"
                    background: Rectangle { color: "#313244"; radius: 4; border.color: "#45475a" }
                    text: "00"
                    maximumLength: 2
                    font.family: "Monospace"
                    implicitWidth: 45
                    horizontalAlignment: TextInput.AlignHCenter
                }

                Rectangle { width: 1; height: 20; color: theme.border }

                StyledButton
                {
                    text: "▶ Single Step"
                    accent: "#a6e3a1"
                    enabled: !vm.isHalted
                    onClicked: vm.step()
                }

               StyledButton
                {
                    text: "⚡ Run All"
                    accent: "#f9e2af"
                    enabled: !vm.isHalted
                    onClicked: vm.runAll()
                }

                StyledButton
                {
                    text: "🔄 Clear / Reset"
                    accent: "#f38ba8"
                    onClicked: vm.reset()
                }

                Item { Layout.fillWidth: true }

                Label 
                {
                    text: "PC: " + vm.pc + "  |  IR: " + vm.ir
                    font.bold: true
                    font.pixelSize: 15
                    font.family: "Monospace"
                    color: theme.accentYellow
                }
            }
        }

        // ==========================================
        // 2. Main Grid: Registers (Left) + 16x16 Memory Matrix (Right)
        // ==========================================
        RowLayout 
        {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12

            // Panel اليسار: المسجلات + Screen Output
            ColumnLayout 
            {
               Layout.fillWidth: false
               Layout.preferredWidth: 280
               Layout.minimumWidth: 280
               Layout.maximumWidth: 280
               Layout.fillHeight: true
                spacing: 10

                // CPU Registers (16 General Purpose Registers)
                Rectangle 
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: theme.bgCard
                    radius: 8
                    border.color: theme.border

                    ColumnLayout 
                    {
                        anchors.fill: parent
                        anchors.margins: 10
                        Label { text: "CPU Registers (Click to Edit)"; color: theme.accentBlue; font.bold: true }

                        GridView 
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            cellWidth: 125
                            cellHeight: 34
                            clip: true
                            model: vm.registers

                            delegate: Rectangle 
                            {
                                width: 118; height: 30
                                color: theme.bgInput
                                radius: 4
                                border.color: theme.border

                                MouseArea 
                                {
                                    anchors.fill: parent
                                    cursorShape: Qt.PointingHandCursor
                                    hoverEnabled: true
                                    onClicked: 
                                    {
                                        editDialog.isRegister = true
                                        editDialog.targetIndex = index
                                        editInput.text = modelData.split("0x")[1]
                                        editDialog.open()
                                    }
                                }

                                RowLayout 
                                {
                                    anchors.fill: parent
                                    anchors.margins: 6
                                    Text { text: "R" + index.toString(16).toUpperCase(); color: theme.accentBlue; font.bold: true; font.family: "Monospace" }
                                    Item { Layout.fillWidth: true }
                                    Text { text: modelData; color: theme.accentGreen; font.bold: true; font.family: "Monospace" }
                                }
                            }
                        }
                    }
                }

                // Screen Terminal Output (Addr 0xFF)
                Rectangle 
                {
                    Layout.fillWidth: true
                    implicitHeight: 140
                    color: "#0d0e15"
                    radius: 8
                    border.color: theme.accentGreen

                    ColumnLayout 
                    {
                        anchors.fill: parent
                        anchors.margins: 8
                        Label { text: "Screen Display (Mapped at 0xFF)"; color: theme.accentGreen; font.bold: true; font.pixelSize: 11 }
                        ScrollView 
                        {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            TextArea 
                            {
                                text: vm.screenText
                                readOnly: true
                                wrapMode: TextArea.Wrap
                                color: theme.accentGreen
                                font.family: "Monospace"
                                font.pixelSize: 13
                                background: null
                            }
                        }
                    }
                }
            }

            // Panel اليمين: الذاكرة المصممة كمصفوفة 16×16 بالتمام والكمال
           // Panel اليمين: الذاكرة 16×16 بتملّا المساحة المتاحة
           Rectangle
           {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 16 * 40 + 20
                color: theme.bgCard
                radius: 8
                border.color: theme.border

                ColumnLayout
                {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 4

                    Label { text: "Main Memory Matrix (256 Bytes — 16x16 Grid)"; color: theme.accentYellow; font.bold: true }

                    // Header الأعمدة (0 to F)
                    Row
                    {
                        spacing: 0
                        Repeater
                        {
                            model: 16
                            Rectangle
                            {
                                width: memoryGrid.cellWidth; height: 20
                                color: "transparent"
                                Text
                                {
                                    anchors.centerIn: parent
                                    text: index.toString(16).toUpperCase()
                                    color: theme.accentBlue
                                    font.bold: true
                                    font.family: "Monospace"
                                }
                            }
                        }
                    }

                    GridView
                    {
                        id: memoryGrid
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        cellWidth: Math.max(1, Math.floor(width / 16))
                        cellHeight: Math.max(1, Math.floor(height / 16))
                        clip: true
                        interactive: false
                        model: vm.memory

                        delegate: Rectangle
                        {
                            id: cellRect
                            width: memoryGrid.cellWidth - 2
                            height: memoryGrid.cellHeight - 2
                            property bool isPC: index === parseInt(vm.pc, 16)
                            color: isPC ? theme.accentRed : theme.bgInput
                            radius: 4
                            border.color: isPC ? "#ffffff" : theme.border
                            border.width: isPC ? 2 : 1

                            MouseArea
                            {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                hoverEnabled: true
                                onClicked:
                                {
                                    editDialog.isRegister = false
                                    editDialog.targetIndex = index
                                    editInput.text = modelData
                                    editDialog.open()
                                }
                            }

                            Text
                            {
                                anchors.centerIn: parent
                                text: modelData
                                color: cellRect.isPC ? "#11111b" : theme.textPrimary
                                font.bold: true
                                font.family: "Monospace"
                                font.pixelSize: Math.max(10, Math.min(memoryGrid.cellHeight * 0.45, 22))
                            }
                        }
                    }
                }
            }
        }

        // ==========================================
        // 3. Bottom Status Bar
        // ==========================================
        Rectangle 
        {
            Layout.fillWidth: true
            implicitHeight: 30
            color: "#11111b"
            radius: 4

            RowLayout 
            {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                Text 
                {
                    text: vm.statusMessage
                    color: vm.statusMessage.indexOf("Error") !== -1 ? theme.accentRed : theme.accentGreen
                    font.bold: true
                    font.pixelSize: 12
                }
                Item { Layout.fillWidth: true }
                Text 
                {
                    text: vm.isHalted ? "Status: HALTED" : "Status: READY"
                    color: vm.isHalted ? theme.accentRed : theme.accentGreen
                    font.bold: true
                }
            }
        }
    }
}