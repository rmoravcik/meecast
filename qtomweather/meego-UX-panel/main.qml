import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Components 0.1 as UX
import "/opt/com.meecast.omweather/lib/OmweatherPlugin" 0.1

Window {
    id: scene
    showsearch: false
    title: "OMWeather Settings"

    Component.onCompleted: {
        applicationPage = appPageComponent
    }
    Config {id: config}
    Component {
        id: appPageComponent
        ApplicationPage {
            id: appPage
            title: "OMWeather Settings"

            Item {
                id: main

                parent: appPage.content
                anchors.fill: parent
                Column {
                    width: parent.width


                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width
                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("Temperature Units")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        UX.DropDown {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            title: config.TemperatureUnit()
                            model: config.TemperatureUnits()
                            width: 300
                            titleColor: black
                            replaceDropDownTitle: true
                            onTriggered: {
                                config.TemperatureUnit(model[index])
                            }
                        }
                    }
                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("Wind speed Units")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        UX.DropDown {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            title: config.WindSpeedUnit()
                            model: config.WindSpeedUnits()
                            width: 300
                            titleColor: black
                            replaceDropDownTitle: true
                            onTriggered: {
                                config.WindSpeedUnit(model[index])
                            }
                        }
                    }
                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("IconSet")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        UX.DropDown {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            title: config.iconSet()
                            model: config.icons()
                            //model: ["meecast", "glance", "other"]
                            width: 300
                            titleColor: black
                            replaceDropDownTitle: true
                            onTriggered: {
                                //txt.text = payload[index] + " = " + data + " - " + model[index];
                                config.iconSet(model[index])
                            }
                        }
                    }
                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("Update interval")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        UX.DropDown {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            title: config.UpdatePeriod()
                            model: config.UpdatePeriods()
                            width: 300
                            titleColor: black
                            replaceDropDownTitle: true
                            onTriggered: {
                                //txt.text = payload[index] + " = " + data + " - " + model[index];
                                config.UpdatePeriod(model[index])
                            }
                        }
                    }
                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("Auto-update on connection")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }

                        UX.ToggleButton {
                            on: config.UpdateConnect()
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 10

                            onToggled: {
                                config.UpdateConnect(isOn);
                            }
                        }

                    }
                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("Stations") + " >"
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                appPage.addApplicationPage(stationsPageComponent);
                            }
                        }

                    }

                    UX.Button{
                        height: 40
                        text: qsTr("Save")
                        onClicked: {
                            config.saveConfig();
                        }
                    }
                }
            }
        }
    }
    Component {
        id: stationsPageComponent
        ApplicationPage {
            id: stationsPage
	    title: qsTr("OMWeather Stations Settings")

            UX.ModalDialog {
                id: newstation
                title: qsTr("New Station")
                showAcceptButton: true
                showCancelButton: true
                acceptButtonText: qsTr("OK")
                cancelButtonText: qsTr("Cancel")
                width: 800
                height: 6 * 77
                content: Item {
                    width: 800
                    height: 6 * 77
                    Column {
                        width: parent.width


                        Image {
                            source: "image://theme/pulldown_box"
                            width: parent.width
                            Text {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: qsTr("Sources")
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            UX.DropDown {
                                id: source
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                //title: config.TemperatureUnit()
                                model: config.Sources()
                                width: 500
                                titleColor: black
                                replaceDropDownTitle: true
                                onTriggered: {
                                    country.model = config.Countries(model[index], false);
                                    country.payload = config.Countries(model[index], true);
                                    country.title = "";
                                }
                            }
                        }
                        Image {
                            source: "image://theme/pulldown_box"
                            width: parent.width

                            Text {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: qsTr("Countries")
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            UX.DropDown {
                                id: country
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                //title: config.WindSpeedUnit()
                                model: config.Countries(source.title, false)
                                payload: config.Countries(source.title, true)
                                width: 500
                                titleColor: black
                                replaceDropDownTitle: true
                                onTriggered: {
                                    region.model = config.Regions(payload[index], false);
                                    region.payload = config.Regions(payload[index], true);
                                    region.title = "";
                                    //console.log(country.selectedItem+" "+country.selectedTitle);
                                    //stationsPage.title = country.selectedIndex+" "+country.selectedTitle;
                                }
                            }
                        }
                        Image {
                            source: "image://theme/pulldown_box"
                            width: parent.width

                            Text {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: qsTr("Region")
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            UX.DropDown {
                                id: region
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                //title: config.WindSpeedUnit()
                                //model: config.Regions(country.payload, false)
                                //payload: config.Regions(source.payload, true)
                                width: 500
                                titleColor: black
                                replaceDropDownTitle: true
                                onTriggered: {
                                    city.model = config.Cities(payload[index], false);
                                    city.payload = config.Cities(payload[index], true);
                                    city.title = "";
                                }
                            }
                        }
                        Image {
                            source: "image://theme/pulldown_box"
                            width: parent.width

                            Text {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: qsTr("City")
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            UX.DropDown {
                                id: city
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                //title: config.WindSpeedUnit()
                                //model: config.Regions(country.payload, false)
                                //payload: config.Regions(source.payload, true)
                                width: 500
                                titleColor: black
                                replaceDropDownTitle: true
                                onTriggered: {

                                }
                            }
                        }
                    }

                }
                onAccepted: {

                }
                onRejected: {
                }
            }

            Item {
                parent: stationsPage.content
                anchors.fill: parent
                ListView {
		    id: stationlist
                    height: config.StationsCount() * 77
                    width: parent.width
                    model: config.Stations()
                    delegate:
                        Image {
                            id: img
                            source: "image://theme/pulldown_box"
                            width: parent.width

                            Text {
				id: txt
                                //anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: modelData
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }

                            UX.Button {
				id: but
				height: 40
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                text: qsTr("Delete")
                                onClicked: {
                                        //but.text = config.Stations().size()
                                }
                            }

                        }

                }
                UX.Button {
                    //height: 40
                    anchors.top: stationlist.bottom
                    text: qsTr("Add new station")
                    onClicked: {
                        newstation.show();
                    }
                }
            }
        }
    }
}



