import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3
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

    FileDialog 
    {
        id: fileDialog
        title: "Select Vole Program File"
        nameFilters: ["Text Files (*.txt)", "All Files (*)"]
        onAccepted: {
            var addr = parseInt(startAddrInput.text, 16)
            if (isNaN(addr)) addr = 0
            vm.loadProgram(fileDialog.fileUrl, addr)
        }
    }

    // نافذة حوارية لتعديل الذاكرة والمسجلات (Overwrite Feature)
    Dialog 
    {
        id: editDialog
        title: "Edit Value (Hex)"
        standardButtons: Dialog.Ok | Dialog.Cancel
        modal: true
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        property bool isRegister: true
        property int targetIndex: 0

        ColumnLayout 
        {
            spacing: 10
            Label 
            {
                text: editDialog.isRegister ? "Set Value for Register R" + editDialog.targetIndex.toString(16).toUpperCase() : "Set Value for Memory [0x" + editDialog.targetIndex.toString(16).toUpperCase().padStart(2, '0') + "]"
                color: theme.textPrimary
            }
            TextField 
            {
                id: editInput
                text: "00"
                maximumLength: 2
                font.family: "Monospace"
                font.bold: true
                Layout.fillWidth: true
            }
        }

        onAccepted: 
        {
            if (isRegister) 
            {
                vm.setRegister(targetIndex, editInput.text)
            } 
            else 
            {
                vm.setMemory(targetIndex, editInput.text)
            }
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

                Button 
                {
                    text: "📁 Load Program"
                    onClicked: fileDialog.open()
                }

                Label { text: "Start Addr (Hex):"; color: theme.textSecondary }
                TextField 
                {
                    id: startAddrInput
                    text: "00"
                    maximumLength: 2
                    font.family: "Monospace"
                    implicitWidth: 45
                    horizontalAlignment: TextInput.AlignHCenter
                }

                Rectangle { width: 1; height: 20; color: theme.border }

                Button 
                {
                    text: "▶ Single Step"
                    enabled: !vm.isHalted
                    onClicked: vm.step()
                }

                Button 
                {
                    text: "⚡ Run All"
                    enabled: !vm.isHalted
                    onClicked: vm.run()
                }

                Button 
                {
                    text: "🔄 Clear / Reset"
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
                Layout.preferredWidth: 280
                Layout.
                fillHeight: true
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
                                    onClicked: 
                                    {
                                        editDialog.isRegister = true
                                        editDialog.targetIndex = index
                                        editInput.text = modelData
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
                    spacing: 4

                    Label { text: "Main Memory Matrix (256 Bytes — 16x16 Grid)"; color: theme.accentYellow; font.bold: true }

                    // Header الأعمدة (0 to F)
                    Row 
                    {
                        spacing: 2
                        Item { width: 25; height: 20 } // فاصل عمود الصفوف
                        Repeater 
                        {
                            model: 16
                            Rectangle 
                            {
                                width: 44; height: 20
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

                    // مصفوفة الـ 16×16 مع عناوين الصفوف (0_ to F_)
                    GridView 
                    {
                        id: memoryGrid
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        cellWidth: 46
                        cellHeight: 32
                        clip: true
                        model: vm.memory

                        delegate: Rectangle 
                        {
                            id: cellRect
                            width: 44; height: 30
                            property bool isPC: index === parseInt(vm.pc, 16)
                            color: isPC ? theme.accentRed : theme.bgInput
                            radius: 4
                            border.color: isPC ? "#ffffff" : theme.border
                            border.width: isPC ? 2 : 1

                            MouseArea 
                            {
                                anchors.fill: parent
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
                                font.pixelSize: 12
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