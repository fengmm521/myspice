//
//  UIIspice.h
//  game4
//
//  Created by Junpeng Zhang on 2/3/15.
//  本代码由工具生成
//
//资源加载适合cocos2d-x 3.2版本的cocostuido 1.6 for windows
//

#ifndef __game4__UIIspice__
#define __game4__UIIspice__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

class DeviceShowBase;  //电子元件基类
class UIBGLine;        //背景线条
class WorkerMainLayer; //电路图

class UIIspice : public Layer
{
protected:
    UIIspice();
public:
    virtual ~UIIspice();
    virtual void close(); //界面关闭
    //获取下次初始化界面时的参数数据,Ref可以是　　cocos2d字典(__Dictionary)，数组(__Array)，或者字符串(__String),或者其他继承自Ref的对象
    virtual cocos2d::Ref* getUIConfData();
    static UIIspice* createFromManger(Ref* dat);
    static UIIspice* create();
    static UIIspice* create(int uiID);
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    Layout* m_rootLayout;
    
    UIBGLine* m_bgLine;
    
    DeviceShowBase* m_touchDevice;
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchMoved(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
//button
    Button* m_mido_Btn; //Panel_14+btn_mido ,Panel+Button
    Button* m_Button_230Btn; //Panel_14+layer_downleft_set+Button_230,Panel+Panel+Button
//CheckBox
    CheckBox* m_cbox_1Chbox; //Panel_14+cbox_1,Panel+CheckBox
    void checkBoxcbox_1Selected();
    void checkBoxcbox_1UnSelected();
    CheckBox* m_cbox_2Chbox; //Panel_14+cbox_2,Panel+CheckBox
    void checkBoxcbox_2Selected();
    void checkBoxcbox_2UnSelected();
    CheckBox* m_cbox_3Chbox; //Panel_14+cbox_3,Panel+CheckBox
    void checkBoxcbox_3Selected();
    void checkBoxcbox_3UnSelected();
    CheckBox* m_cbox_4Chbox; //Panel_14+cbox_4,Panel+CheckBox
    void checkBoxcbox_4Selected();
    void checkBoxcbox_4UnSelected();
    CheckBox* m_cbox_5Chbox; //Panel_14+cbox_5,Panel+CheckBox
    void checkBoxcbox_5Selected();
    void checkBoxcbox_5UnSelected();
    CheckBox* m_cbox_6Chbox; //Panel_14+cbox_6,Panel+CheckBox
    void checkBoxcbox_6Selected();
    void checkBoxcbox_6UnSelected();
//ImageView
    ImageView* m_img_xImage; //Panel_14+cbox_1+img_x,Panel+CheckBox+ImageVie
    ImageView* m_img_ringfloorImage; //Panel_14+layer_ring +img_ringfloor,Panel+Panel+ImageView
    ImageView* m_img_ringfloagImage; //Panel_14+layer_ring +img_ringfloag,Panel+Panel+ImageView
    ImageView* m_img_ringpanImage; //Panel_14+layer_ring +img_ringpan,Panel+Panel+ImageView
    ImageView* m_img_ringpointImage; //Panel_14+layer_ring +Panel_25+img_ringpoint,Panel+Panel+Panel+ImageView
    ImageView* m_img_lablebgImage; //Panel_14+layer_ring +img_lablebg,Panel+Panel+ImageView
    ImageView* m_img_m1Image; //Panel_14+layer_mido+img_m1,Panel+Panel+ImageView
    ImageView* m_img_m2Image; //Panel_14+layer_mido+img_m2,Panel+Panel+ImageView
    ImageView* m_img_m3Image; //Panel_14+layer_mido+img_m3,Panel+Panel+ImageView
    ImageView* m_img_m4Image; //Panel_14+layer_mido+img_m4,Panel+Panel+ImageView
    ImageView* m_img_m5Image; //Panel_14+layer_mido+img_m5,Panel+Panel+ImageView
    ImageView* m_img_m6Image; //Panel_14+layer_mido+img_m6,Panel+Panel+ImageView
    ImageView* m_img_devicebgImage; //Panel_14+layer_downleft+img_devicebg,Panel+Panel+ImageView
    ImageView* m_img_deviceiconImage; //Panel_14+layer_downleft+img_deviceicon,Panel+Panel+ImageView
    ImageView* m_img_lableNamebgImage; //Panel_14+layer_downleft+img_lableNamebg,Panel+Panel+ImageView
    ImageView* m_img_lablevbg1Image; //Panel_14+layer_downleft+img_lablevbg1,Panel+Panel+ImageView
    ImageView* m_img_lablevbg2Image; //Panel_14+layer_downleft+img_lablevbg2,Panel+Panel+ImageView
    ImageView* m_img_lablevbg3Image; //Panel_14+layer_downleft+img_lablevbg3,Panel+Panel+ImageView
    ImageView* m_img_lablevbg4Image; //Panel_14+layer_downleft+img_lablevbg4,Panel+Panel+ImageView
    ImageView* m_img_lablevbg5Image; //Panel_14+layer_downleft+img_lablevbg5,Panel+Panel+ImageView
    ImageView* m_img_lablevbg6Image; //Panel_14+layer_downleft+img_lablevbg6 ,Panel+Panel+ImageView
    ImageView* m_img_lablevbg7Image; //Panel_14+layer_downleft+img_lablevbg7,Panel+Panel+ImageView
    ImageView* m_img_lablevbg8Image; //Panel_14+layer_downleft+img_lablevbg8,Panel+Panel+ImageView
    ImageView* m_img_lablespicemodeImage; //Panel_14+layer_downleft+img_lablespicemode,Panel+Panel+ImageView
    ImageView* m_img_line0Image; //Panel_14+layer_wavecell+img_line0,Panel+Panel+ImageView
    ImageView* m_img_line1Image; //Panel_14+layer_wavecell+img_line1,Panel+Panel+ImageView
    ImageView* m_img_line2Image; //Panel_14+layer_wavecell+img_line2,Panel+Panel+ImageView
    ImageView* m_img_line3Image; //Panel_14+layer_wavecell+img_line3,Panel+Panel+ImageView
    ImageView* m_img_line4Image; //Panel_14+layer_wavecell+img_line4,Panel+Panel+ImageView
    ImageView* m_img_line5Image; //Panel_14+layer_wavecell+img_line5,Panel+Panel+ImageView
    ImageView* m_img_tablebgImage; //Panel_14+layer_devicecell+img_tablebg,Panel+Panel+ImageView
    ImageView* m_img_lineImage; //Panel_14+layer_devicecell+img_line,Panel+Panel+ImageView
    ImageView* m_img_circuitbgImage; //Panel_14+layer_devicecell+img_circuitbg,Panel+Panel+ImageView
    ImageView* m_img_deviceImage; //Panel_14+layer_devicecell+img_device,Panel+Panel+ImageView
//TextLabel
    Text* m_Label_nameText; //Panel_14+layer_downleft+img_lableNamebg+Label_name,Panel+Panel+ImageView+Label
    Text* m_Label_circuitnameText;
    
    Text* m_Label_vText; //Panel_14+layer_downleft+img_lablevbg1+Label_v,Panel+Panel+ImageView+Label
    Text* m_Label_v1Text; //Panel_14+layer_downleft+img_lablespicemode+Label_v1,Panel+Panel+ImageView+Label
    Text* m_Label_v2Text; //Panel_14+layer_downleft+img_lablespicemode+Label_v2,Panel+Panel+ImageView+Label
    Text* m_Label_v3Text; //Panel_14+layer_downleft+img_lablespicemode+Label_v3,Panel+Panel+ImageView+Label
    Text* m_lab_line0Text; //Panel_14+layer_wavecell+lab_line0,Panel+Panel+Label
    Text* m_lab_line1Text; //Panel_14+layer_wavecell+lab_line1,Panel+Panel+Label
    Text* m_lab_line2Text; //Panel_14+layer_wavecell+lab_line2,Panel+Panel+Label
    Text* m_lab_line3Text; //Panel_14+layer_wavecell+lab_line3,Panel+Panel+Label
    Text* m_lab_line4Text; //Panel_14+layer_wavecell+lab_line4,Panel+Panel+Label
    Text* m_lab_titleText; //Panel_14+layer_wavecell+lab_title,Panel+Panel+Label
    Text* m_lable_name1Text; //Panel_14+layer_devicecell+lable_name1,Panel+Panel+Label
    Text* m_lable_name2Text; //Panel_14+layer_devicecell+lable_name2,Panel+Panel+Label
    
    Text* m_Label_cmdlineText;
    
    std::string m_TextField_nameStrValue;//输入框中的值
    void textFieldTextField_nameTouchEvent(Ref* obj, TextField::EventType type);
    TextField* m_TextField_nameTextInput; //Panel_14+layer_downleft+img_lablevbg1+TextField_v,Panel+Panel+ImageView+TextField
    std::string m_TextField_v1StrValue;//输入框中的值
    void textFieldTextField_v1TouchEvent(Ref* obj, TextField::EventType type);
    TextField* m_TextField_v1TextInput; //Panel_14+layer_downleft+img_lablevbg2+TextField_v,Panel+Panel+ImageView+TextField
    std::string m_TextField_v2StrValue;//输入框中的值
    void textFieldTextField_v2TouchEvent(Ref* obj, TextField::EventType type);
    TextField* m_TextField_v2TextInput; //Panel_14+layer_downleft+img_lablevbg3+TextField_v,Panel+Panel+ImageView+TextField
    std::string m_TextField_v3StrValue;//输入框中的值
    void textFieldTextField_v3TouchEvent(Ref* obj, TextField::EventType type);
    TextField* m_TextField_v3TextInput; //Panel_14+layer_downleft+img_lablevbg4+TextField_v,Panel+Panel+ImageView+TextField
    std::string m_TextField_v4StrValue;//输入框中的值
    void textFieldTextField_v4TouchEvent(Ref* obj, TextField::EventType type);
    TextField* m_TextField_v4TextInput; //Panel_14+layer_downleft+img_lablevbg5+TextField_v,Panel+Panel+ImageView+TextField
    std::string m_TextField_v5StrValue;//输入框中的值
    void textFieldTextField_v5TouchEvent(Ref* obj, TextField::EventType type);
    TextField* m_TextField_v5TextInput; //Panel_14+layer_downleft+img_lablevbg6 +TextField_v,Panel+Panel+ImageView+TextField
    std::string m_TextField_v6StrValue;//输入框中的值
    void textFieldTextField_v6TouchEvent(Ref* obj, TextField::EventType type);
    TextField* m_TextField_v6TextInput; //Panel_14+layer_downleft+img_lablevbg7+TextField_v,Panel+Panel+ImageView+TextField
    std::string m_TextField_v7StrValue;//输入框中的值
    void textFieldTextField_v7TouchEvent(Ref* obj, TextField::EventType type);
    TextField* m_TextField_v7TextInput; //Panel_14+layer_downleft+img_lablevbg8+TextField_v,Panel+Panel+ImageView+TextField
    std::string m_TextField_v8StrValue;//输入框中的值
    void textFieldTextField_v8TouchEvent(Ref* obj, TextField::EventType type);
    TextField* m_TextField_v8TextInput; //Panel_14+layer_downleft+img_lablespicemode+TextField_v,Panel+Panel+ImageView+TextField
    std::string m_TextField_v9StrValue;//输入框中的值
    void textFieldTextField_v9TouchEvent(Ref* obj, TextField::EventType type);
    TextField* m_TextField_v9TextInput; //Panel_14+layer_downleft_set+img_lableNamebg
    std::string m_TextField_modeStrValue;//输入框中的值
    void textFieldTextField_modeTouchEvent(Ref* obj, TextField::EventType type);
//Panel
    Layout* m_ringLayer; //Panel_14+layer_ring ,Panel+Panel
    void initLayoutlayer_ring();
    Layout* m_Panel_25Layout; //Panel_14+layer_ring +Panel_25,Panel+Panel+Panel
    void initLayoutPanel_25();
    Layout* m_layer_midoLayout; //Panel_14+layer_mido,Panel+Panel
    void initLayoutlayer_mido();
    Layout* m_layer_downleftLayout; //Panel_14+layer_downleft,Panel+Panel
    void initLayoutlayer_downleft();
    Layout* m_layer_panbgLayout; //Panel_14+layer_panbg,Panel+Panel
    void initLayoutlayer_panbg();
    Layout* m_layer_wavecellLayout; //Panel_14+layer_wavecell,Panel+Panel
    void initLayoutlayer_wavecell();
    Layout* m_layer_devicecellLayout; //Panel_14+layer_devicecell,Panel+Panel
    void initLayoutlayer_devicecell();
    Layout* m_layer_cmdcellLayout; //Panel_14+layer_cmdcell,Panel+Panel
    void initLayoutlayer_cmdcell();
    Layout* m_layer_setcellLayout; //Panel_14+layer_setcell,Panel+Panel
    void initLayoutlayer_setcell();
    Layout* m_layer_downleft_setLayout; //Panel_14+layer_downleft_set,Panel+Panel
    void initLayoutlayer_downleft_set();
    Layout* m_layer_opencellLayout; //Panel_14+layer_opencell,Panel+Panel
    void initLayoutlayer_opencell();
//ListView
    ListView* m_waveListView; //Panel_14+listview_wave,Panel+ListView
    int m_prePresslistview_wave;
    std::vector<int> m_listlistview_waveDataVect;
    void initListViewlistview_wave();
    void setPanellistview_waveItemHide();
    void setPanellistview_waveItemShow(int n);
    void setListViewlistview_waveItem(Widget* w,int n);//初始化子控件
    void listScrollViewlistview_waveEvent(Ref *pSender, ui::ScrollView::EventType type);//滚动图层回调函数,控制滑块
    void listview_waveEvent(Ref *pSender, ui::ListView::EventType type);
    
    ListView* m_deviceListView; //Panel_14+listview_device,Panel+ListView
    int m_prePresslistview_device;
    std::vector<int> m_listlistview_deviceDataVect;
    void initListViewlistview_device();
    void setPanellistview_deviceItemHide();
    void setListView_deviceItemShow(int n);
    void setListViewlistview_deviceItem(Widget* w,int n);//初始化子控件
    void listScrollViewlistview_deviceEvent(Ref *pSender, ui::ScrollView::EventType type);//滚动图层回调函数,控制滑块
    void listview_deviceEvent(Ref *pSender, ui::ListView::EventType type);
    
    ListView* m_cmdlineListView; //Panel_14+ListView_cmdline,Panel+ListView
    int m_prePressListView_cmdline;
    std::vector<int> m_listListView_cmdlineDataVect;
    void initListViewListView_cmdline();
    void setPanelListView_cmdlineItemHide();
    void setPanelListView_cmdlineItemShow(int n);
    void setListViewListView_cmdlineItem(Widget* w,int n);//初始化子控件
    void listScrollViewListView_cmdlineEvent(Ref *pSender, ui::ScrollView::EventType type);//滚动图层回调函数,控制滑块
    void ListView_cmdlineEvent(Ref *pSender, ui::ListView::EventType type);
    
    ListView* m_downleft_openListView; //Panel_14+ListView_downleft_open,Panel+ListView
    int m_prePressListView_downleft_open;
    std::vector<int> m_listListView_downleft_openDataVect;
    void initListViewListView_downleft_open();
    void setPanelListView_downleft_openItemHide();
    void setPanelListView_downleft_openItemShow(int n);
    void setListViewListView_downleft_openItem(Widget* w,int n);//初始化子控件
    void listScrollViewListView_downleft_openEvent(Ref *pSender, ui::ScrollView::EventType type);//滚动图层回调函数,控制滑块
    void ListView_downleft_openEvent(Ref *pSender, ui::ListView::EventType type);

    void onTouchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
    void onWidgetTouchBeigen(cocos2d::Ref *pSender);
    void onWidgetTouchMove(cocos2d::Ref *pSender);
    void onWidgetTouchEnd(cocos2d::Ref *pSender);
    void onWidgetTouchCanceled(cocos2d::Ref *pSender);


    void checkBoxEvent(Ref *pSender,CheckBox::EventType type);
    void onCheckBoxSelectEvent(Ref *pSender);
    void onCheckBoxUnSelectEvent(Ref *pSender);

    //移动电路图
    void moveCircute(Point &po);
private:
    int m_uiID;
    
    Point m_moveStartPoint;
    Point m_movingPoint;
    
    void moveBGLine(Point &po);
    
    //电路图
    WorkerMainLayer* s_workerlayer; 
    void moveMainWorkLayer(Point &po);
    
};

#endif /* defined(__game4__UIIspice__) */
    