//
//  UIIspice.cpp
//  game4
//
//  Created by Junpeng Zhang on 2/3/15.
//  本代码由工具生成
//资源加载适合cocos2d-x 3.2版本的cocostuido 1.6 for windows
//

#include "UIIspice.h"
#include "strHash.h"
#include "UIBGLine.h"

#define kSoundButton "sound/Click_Button.wav"

USING_NS_CC;
using namespace ui;
using namespace std;
UIIspice::UIIspice()
{
    
}
UIIspice::~UIIspice()
{
    
}
cocos2d::Ref* UIIspice::getUIConfData()
{
    __String* str = __String::createWithFormat("%d",m_uiID);
    return str;
}
UIIspice* UIIspice::create(int uiID)
{
    UIIspice* tmp = new UIIspice();
    if(tmp){
        tmp->autorelease();
        tmp->m_uiID = uiID;
        tmp->init();
        return tmp;
    }else{
        return nullptr;
    }
}
// on "init" you need to initialize your instance
bool UIIspice::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    m_rootLayout = dynamic_cast<ui::Layout*>(GUIReader::getInstance()->widgetFromJsonFile("ui/UIIspice.ExportJson"));
    
    m_bgLine = UIBGLine::createBGLine();
    m_rootLayout->addChild(m_bgLine);
    //Button
    m_mido_Btn = dynamic_cast<ui::Button*>(Helper::seekWidgetByName(m_rootLayout, "btn_mido"));
    m_mido_Btn->addTouchEventListener(CC_CALLBACK_2(UIIspice::onTouchEvent, this));
    Widget* tmpxlayer_downleft_set = Helper::seekWidgetByName(m_rootLayout, "layer_downleft_set");
    Button* p_BtnButton_230 = dynamic_cast<ui::Button*>(Helper::seekWidgetByName(tmpxlayer_downleft_set, "Button_230"));
    p_BtnButton_230->setTag(1);
    p_BtnButton_230->addTouchEventListener(CC_CALLBACK_2(UIIspice::onTouchEvent, this));

    //CheckBox
    m_cbox_1Chbox = dynamic_cast<CheckBox*>(Helper::seekWidgetByName(m_rootLayout, "cbox_1"));
    m_cbox_1Chbox->addEventListener(std::bind(&UIIspice::checkBoxEvent,this,std::placeholders::_1,std::placeholders::_2));

    m_cbox_2Chbox = dynamic_cast<CheckBox*>(Helper::seekWidgetByName(m_rootLayout, "cbox_2"));
    m_cbox_2Chbox->addEventListener(std::bind(&UIIspice::checkBoxEvent,this,std::placeholders::_1,std::placeholders::_2));

    m_cbox_3Chbox = dynamic_cast<CheckBox*>(Helper::seekWidgetByName(m_rootLayout, "cbox_3"));
    m_cbox_3Chbox->addEventListener(std::bind(&UIIspice::checkBoxEvent,this,std::placeholders::_1,std::placeholders::_2));

    m_cbox_4Chbox = dynamic_cast<CheckBox*>(Helper::seekWidgetByName(m_rootLayout, "cbox_4"));
    m_cbox_4Chbox->addEventListener(std::bind(&UIIspice::checkBoxEvent,this,std::placeholders::_1,std::placeholders::_2));

    m_cbox_5Chbox = dynamic_cast<CheckBox*>(Helper::seekWidgetByName(m_rootLayout, "cbox_5"));
    m_cbox_5Chbox->addEventListener(std::bind(&UIIspice::checkBoxEvent,this,std::placeholders::_1,std::placeholders::_2));

    m_cbox_6Chbox = dynamic_cast<CheckBox*>(Helper::seekWidgetByName(m_rootLayout, "cbox_6"));
    m_cbox_6Chbox->addEventListener(std::bind(&UIIspice::checkBoxEvent,this,std::placeholders::_1,std::placeholders::_2));

    //ImageView
    Widget* tmpxcbox_1 = Helper::seekWidgetByName(m_rootLayout, "cbox_1");
    ImageView* p_image0 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxcbox_1, "img_x"));
    Widget* tmpxcbox_2 = Helper::seekWidgetByName(m_rootLayout, "cbox_2");
    ImageView* p_image1 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxcbox_2, "img_x"));
    Widget* tmpxcbox_3 = Helper::seekWidgetByName(m_rootLayout, "cbox_3");
    ImageView* p_image2 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxcbox_3, "img_x"));
    Widget* tmpxcbox_4 = Helper::seekWidgetByName(m_rootLayout, "cbox_4");
    ImageView* p_image3 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxcbox_4, "img_x"));
    Widget* tmpxcbox_5 = Helper::seekWidgetByName(m_rootLayout, "cbox_5");
    ImageView* p_image4 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxcbox_5, "img_x"));
    Widget* tmpxcbox_6 = Helper::seekWidgetByName(m_rootLayout, "cbox_6");
    ImageView* p_image5 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxcbox_6, "img_x"));
    Widget* tmpxlayer_ring  = Helper::seekWidgetByName(m_rootLayout, "layer_ring");
    ImageView* p_image6 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_ring , "img_ringfloor"));
    ImageView* p_image7 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_ring , "img_ringfloag"));
    ImageView* p_image8 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_ring , "img_ringpan"));
    ImageView* p_image9 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_ring , "img_ringpoint"));
    ImageView* p_image10 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_ring , "img_lablebg"));
    
    Widget* tmpxlayer_mido = Helper::seekWidgetByName(m_rootLayout, "layer_mido");
    ImageView* p_image11 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_mido, "img_m1"));
    ImageView* p_image12 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_mido, "img_m2"));
    ImageView* p_image13 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_mido, "img_m3"));
    ImageView* p_image14 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_mido, "img_m4"));
    ImageView* p_image15 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_mido, "img_m5"));
    ImageView* p_image16 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_mido, "img_m6"));
    
    Widget* tmpxbtn_mido  = Helper::seekWidgetByName(m_rootLayout, "btn_mido ");
    ImageView* p_image17 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxbtn_mido , "img_x"));
    
    Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    ImageView* p_image18 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_devicebg"));
    ImageView* p_image19 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_deviceicon"));
    ImageView* p_image20 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_lableNamebg"));
    ImageView* p_image21 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_lablevbg1"));
    ImageView* p_image22 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_lablevbg2"));
    ImageView* p_image23 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_lablevbg3"));
    ImageView* p_image24 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_lablevbg4"));
    ImageView* p_image25 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_lablevbg5"));
    ImageView* p_image26 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_lablevbg6 "));
    ImageView* p_image27 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_lablevbg7"));
    ImageView* p_image28 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_lablevbg8"));
    ImageView* p_image29 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft, "img_lablespicemode"));
    Widget* tmpxlayer_wavecell = Helper::seekWidgetByName(m_rootLayout, "layer_wavecell");
    ImageView* p_image30 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "img_line0"));
    ImageView* p_image31 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "img_line1"));
    ImageView* p_image32 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "img_line2"));
    ImageView* p_image33 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "img_line3"));
    ImageView* p_image34 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "img_line4"));
    ImageView* p_image35 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "img_line5"));
    Widget* tmpxlayer_devicecell = Helper::seekWidgetByName(m_rootLayout, "layer_devicecell");
    ImageView* p_image36 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_devicecell, "img_tablebg"));

    ImageView* p_image37 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_devicecell, "img_line"));
   // Widget* tmpxlayer_devicecell = Helper::seekWidgetByName(m_rootLayout, "layer_devicecell");
    ImageView* p_image38 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_devicecell, "img_circuitbg"));
   // Widget* tmpxlayer_devicecell = Helper::seekWidgetByName(m_rootLayout, "layer_devicecell");
    ImageView* p_image39 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_devicecell, "img_device"));
    
    Widget* tmpxlayer_setcell = Helper::seekWidgetByName(m_rootLayout, "layer_setcell");
    ImageView* p_image40 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_setcell, "img_tablebg"));
  //  Widget* tmpxlayer_setcell = Helper::seekWidgetByName(m_rootLayout, "layer_setcell");
    ImageView* p_image41 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_setcell, "img_line"));
  //  Widget* tmpxlayer_downleft_set = Helper::seekWidgetByName(m_rootLayout, "layer_downleft_set");
    ImageView* p_image42 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft_set, "img_devicebg"));
  //  Widget* tmpxlayer_downleft_set = Helper::seekWidgetByName(m_rootLayout, "layer_downleft_set");
    ImageView* p_image43 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft_set, "img_deviceicon"));
  //  Widget* tmpxlayer_downleft_set = Helper::seekWidgetByName(m_rootLayout, "layer_downleft_set");
    ImageView* p_image44 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_downleft_set, "img_lableNamebg"));
    
    Widget* tmpxlayer_opencell = Helper::seekWidgetByName(m_rootLayout, "layer_opencell");
    ImageView* p_image45 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_opencell, "img_devicebg"));
  //  Widget* tmpxlayer_opencell = Helper::seekWidgetByName(m_rootLayout, "layer_opencell");
    ImageView* p_image46 = dynamic_cast<ui::ImageView*>(Helper::seekWidgetByName(tmpxlayer_opencell, "img_deviceicon"));
    //Label
  //  Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_nameText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_name"));
  //  Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_vText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v"));
  //  Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_vText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v"));
  //  Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_vText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v"));
 //   Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_vText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v"));
 //   Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_vText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v"));
 //   Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_vText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v"));
 //   Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_vText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v"));
 //   Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_vText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v"));
 //   Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_v1Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v1"));
 //   Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_v2Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v2"));
 //   Widget* tmpxlayer_downleft = Helper::seekWidgetByName(m_rootLayout, "layer_downleft");
    m_Label_v3Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft, "Label_v3"));
 //   Widget* tmpxlayer_wavecell = Helper::seekWidgetByName(m_rootLayout, "layer_wavecell");
    m_lab_line0Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "lab_line0"));
 //   Widget* tmpxlayer_wavecell = Helper::seekWidgetByName(m_rootLayout, "layer_wavecell");
    m_lab_line1Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "lab_line1"));
 //   Widget* tmpxlayer_wavecell = Helper::seekWidgetByName(m_rootLayout, "layer_wavecell");
    m_lab_line2Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "lab_line2"));
 //   Widget* tmpxlayer_wavecell = Helper::seekWidgetByName(m_rootLayout, "layer_wavecell");
    m_lab_line3Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "lab_line3"));
 //   Widget* tmpxlayer_wavecell = Helper::seekWidgetByName(m_rootLayout, "layer_wavecell");
    m_lab_line4Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "lab_line4"));
  //  Widget* tmpxlayer_wavecell = Helper::seekWidgetByName(m_rootLayout, "layer_wavecell");
    m_lab_titleText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_wavecell, "lab_title"));
 //   Widget* tmpxlayer_devicecell = Helper::seekWidgetByName(m_rootLayout, "layer_devicecell");
    m_lable_name1Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_devicecell, "lable_name1"));
 //   Widget* tmpxlayer_devicecell = Helper::seekWidgetByName(m_rootLayout, "layer_devicecell");
    m_lable_name2Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_devicecell, "lable_name2"));
    
    Widget* tmpxlayer_cmdcell = Helper::seekWidgetByName(m_rootLayout, "layer_cmdcell");
    m_Label_cmdlineText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_cmdcell, "Label_cmdline"));
  //  Widget* tmpxlayer_setcell = Helper::seekWidgetByName(m_rootLayout, "layer_setcell");
    m_lable_name1Text = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_setcell, "lable_name1"));
   // Widget* tmpxlayer_downleft_set = Helper::seekWidgetByName(m_rootLayout, "layer_downleft_set");
    m_Label_nameText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_downleft_set, "Label_name"));
   // Widget* tmpxlayer_opencell = Helper::seekWidgetByName(m_rootLayout, "layer_opencell");
    m_Label_circuitnameText = dynamic_cast<Text*>(Helper::seekWidgetByName(tmpxlayer_opencell, "Label_circuitname"));
    //TextField
    m_TextField_nameTextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_name"));
    m_TextField_nameTextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_nameTouchEvent,this));

    m_TextField_v1TextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_v"));
    m_TextField_v1TextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_v1TouchEvent,this));

    m_TextField_v2TextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_v"));
    m_TextField_v2TextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_v2TouchEvent,this));

    m_TextField_v3TextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_v"));
    m_TextField_v3TextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_v3TouchEvent,this));

    m_TextField_v4TextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_v"));
    m_TextField_v4TextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_v4TouchEvent,this));

    m_TextField_v5TextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_v"));
    m_TextField_v5TextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_v5TouchEvent,this));

    m_TextField_v6TextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_v"));
    m_TextField_v6TextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_v6TouchEvent,this));

    m_TextField_v7TextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_v"));
    m_TextField_v7TextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_v7TouchEvent,this));

    m_TextField_v8TextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_v"));
    m_TextField_v8TextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_v8TouchEvent,this));

    m_TextField_v9TextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_v"));
    m_TextField_v9TextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_v9TouchEvent,this));

    m_TextField_nameTextInput = dynamic_cast<TextField*>(Helper::seekWidgetByName(m_rootLayout, "TextField_name"));
    m_TextField_nameTextInput->addEventListener(CC_CALLBACK_2(UIIspice::textFieldTextField_nameTouchEvent,this));

    //Panel
    m_ringLayer = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "layer_ring"));
    this->initLayoutlayer_ring();
    m_Panel_25Layout = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "Panel_25"));
    this->initLayoutPanel_25();
    m_layer_midoLayout = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "layer_mido"));
    this->initLayoutlayer_mido();
    m_layer_downleftLayout = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "layer_downleft"));
    this->initLayoutlayer_downleft();
    m_layer_panbgLayout = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "layer_panbg"));
    this->initLayoutlayer_panbg();
    m_layer_wavecellLayout = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "layer_wavecell"));
    this->initLayoutlayer_wavecell();
    m_layer_devicecellLayout = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "layer_devicecell"));
    this->initLayoutlayer_devicecell();
    m_layer_cmdcellLayout = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "layer_cmdcell"));
    this->initLayoutlayer_cmdcell();
    m_layer_setcellLayout = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "layer_setcell"));
    this->initLayoutlayer_setcell();
    m_layer_downleft_setLayout = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "layer_downleft_set"));
    this->initLayoutlayer_downleft_set();
    m_layer_opencellLayout = dynamic_cast<Layout*>(Helper::seekWidgetByName(m_rootLayout, "layer_opencell"));
    this->initLayoutlayer_opencell();
    
    
    //ListView
    m_prePresslistview_wave = -1;
    m_waveListView = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_rootLayout, "listview_wave"));
    m_waveListView->addEventListener(CC_CALLBACK_2(UIIspice::listview_waveEvent, this));
    m_waveListView->addEventListener(CC_CALLBACK_2(UIIspice::listScrollViewlistview_waveEvent,this));
    this->initListViewlistview_wave();
    
    m_prePresslistview_device = -1;
    m_deviceListView = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_rootLayout, "listview_device"));
    m_deviceListView->addEventListener(CC_CALLBACK_2(UIIspice::listview_deviceEvent, this));
    m_deviceListView->addEventListener(CC_CALLBACK_2(UIIspice::listScrollViewlistview_deviceEvent, this));
    this->initListViewlistview_device();
    
    m_prePressListView_cmdline = -1;
    m_cmdlineListView = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_rootLayout, "ListView_cmdline"));
    m_cmdlineListView->addEventListener(CC_CALLBACK_2(UIIspice::ListView_cmdlineEvent, this));
    m_cmdlineListView->addEventListener(CC_CALLBACK_2(UIIspice::listScrollViewListView_cmdlineEvent, this));
    this->initListViewListView_cmdline();
    
    m_prePressListView_downleft_open = -1;
    m_downleft_openListView = dynamic_cast<ListView*>(Helper::seekWidgetByName(m_rootLayout, "ListView_downleft_open"));
    m_downleft_openListView->addEventListener(CC_CALLBACK_2(UIIspice::ListView_downleft_openEvent, this));
    m_downleft_openListView->addEventListener(CC_CALLBACK_2(UIIspice::listScrollViewListView_downleft_openEvent, this));
    this->initListViewListView_downleft_open();


    this->addChild(m_rootLayout);
    m_rootLayout->setTouchEnabled(false);
    
    // 创建一个事件监听器 OneByOne 为单点触摸
    auto listener1 = EventListenerTouchOneByOne::create();
    
    // 设置是否吞没事件，在 onTouchBegan 方法返回 true 时吞没
    listener1->onTouchBegan = CC_CALLBACK_2(UIIspice::onTouchBegan, this);
    listener1->onTouchMoved = CC_CALLBACK_2(UIIspice::onTouchMoved, this);
    listener1->onTouchEnded = CC_CALLBACK_2(UIIspice::onTouchEnded, this);
    listener1->onTouchEnded = CC_CALLBACK_2(UIIspice::onTouchEnded, this);
    listener1->setSwallowTouches(true);
    // _eventDispatcher = director->getEventDispatcher();
    //由于Director是单例，所以_eventDispatcher
    // 获得的就相当于单例对象
    
    
    m_touchDevice = nullptr;//点击的电路
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
    
    return true;
}
bool UIIspice::onTouchBegan(Touch *touch, Event *unused_event)
{
    CCLOG("touch began");
    m_moveStartPoint  = touch->getLocation();
    return true;
}
void UIIspice::onTouchMoved(Touch *touch, Event *unused_event)
{
    m_movingPoint = touch->getLocation();
    
    Point offsetPoint = m_movingPoint;
    offsetPoint.subtract(m_moveStartPoint);
    if(offsetPoint.length() > 20)
    {
        this->moveCircute(offsetPoint);
        
    }
    
    CCLOG("touch move");
}
void UIIspice::onTouchEnded(Touch *touch, Event *unused_event)
{
    
}
void UIIspice::onTouchCancelled(Touch *touch, Event *unused_event)
{
    
}
//移动电路图
void UIIspice::moveCircute(Point &po)
{
    this->moveBGLine(po);          //移动背景线条
    this->moveMainWorkLayer(po);   //移动电路图
}
void UIIspice::moveBGLine(Point &po)
{
    m_bgLine->moveTo(po);
}
//移动电路路
void UIIspice::moveMainWorkLayer(Point &po)
{
    
}

void UIIspice::close()
{
    this->removeFromParent();
}
//buttonEvent

void UIIspice::onTouchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
    //按下、移动、结束、取消
   // string buttonname = btntmp->getName();
    switch (type)
    {
        case Widget::TouchEventType::ENDED:
        {
            this->onWidgetTouchEnd(pSender);
        }
            break;
        case Widget::TouchEventType::BEGAN:
        {
            this->onWidgetTouchBeigen(pSender);
        }
            break;
        case Widget::TouchEventType::MOVED:
        {
            this->onWidgetTouchMove(pSender);
        }
            break;
        case Widget::TouchEventType::CANCELED:
        {
            this->onWidgetTouchCanceled(pSender);
        }
            break;
        default:
            break;
    }

}

void UIIspice::onWidgetTouchBeigen(cocos2d::Ref *pSender)
{
    Widget* btntmp = dynamic_cast<Widget*>(pSender);
    string buttonname = btntmp->getName();
    switch(hash_(buttonname.c_str())){
        case "btn_mido "_hash:
        {

        }
            break;
        case "Button_230"_hash:
        {

        }
            break;
        default:
            break;
    }
}

void UIIspice::onWidgetTouchMove(cocos2d::Ref *pSender)
{
    Widget* btntmp = dynamic_cast<Widget*>(pSender);
    string buttonname = btntmp->getName();
    switch(hash_(buttonname.c_str())){
        case "btn_mido "_hash:
        {

        }
            break;
        case "Button_230"_hash:
        {

        }
            break;
        default:
            break;
    }
}

void UIIspice::onWidgetTouchEnd(cocos2d::Ref *pSender)
{
    Widget* btntmp = dynamic_cast<Widget*>(pSender);
    string buttonname = btntmp->getName();
    switch(hash_(buttonname.c_str())){
        case "btn_mido "_hash:
        {

        }
            break;
        case "Button_230"_hash:
        {

        }
            break;
        default:
            break;
    }
}

void UIIspice::onWidgetTouchCanceled(cocos2d::Ref *pSender)
{
    Widget* btntmp = dynamic_cast<Widget*>(pSender);
    string buttonname = btntmp->getName();
    switch(hash_(buttonname.c_str())){
        case "btn_mido "_hash:
        {

        }
            break;
        case "Button_230"_hash:
        {

        }
            break;
        default:
            break;
    }
}
//CheckBox
void UIIspice::checkBoxEvent(Ref *pSender,ui::CheckBox::EventType type)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(kSoundButton);
    if (type == CheckBox::EventType::SELECTED) {
        this->onCheckBoxSelectEvent(pSender);
    }else{//UNSELECTED
        this->onCheckBoxUnSelectEvent(pSender);
    }
}
 void UIIspice::onCheckBoxSelectEvent(Ref *pSender)
{
    CheckBox* btntmp = dynamic_cast<CheckBox*>(pSender);
    switch(hash_(btntmp->getName().c_str())){
        case "cbox_1"_hash:
        {
            this->checkBoxcbox_1Selected();
        }
            break;
        case "cbox_2"_hash:
        {
            this->checkBoxcbox_2Selected();
        }
            break;
        case "cbox_3"_hash:
        {
            this->checkBoxcbox_3Selected();
        }
            break;
        case "cbox_4"_hash:
        {
            this->checkBoxcbox_4Selected();
        }
            break;
        case "cbox_5"_hash:
        {
            this->checkBoxcbox_5Selected();
        }
            break;
        case "cbox_6"_hash:
        {
            this->checkBoxcbox_6Selected();
        }
            break;
        default:
            break;
        }
}
 void UIIspice::onCheckBoxUnSelectEvent(Ref *pSender)
{
    CheckBox* btntmp = dynamic_cast<CheckBox*>(pSender);
    switch(hash_(btntmp->getName().c_str())){
        case "cbox_1"_hash:
        {
            this->checkBoxcbox_1UnSelected();
        }
            break;
        case "cbox_2"_hash:
        {
            this->checkBoxcbox_2UnSelected();
        }
            break;
        case "cbox_3"_hash:
        {
            this->checkBoxcbox_3UnSelected();
        }
            break;
        case "cbox_4"_hash:
        {
            this->checkBoxcbox_4UnSelected();
        }
            break;
        case "cbox_5"_hash:
        {
            this->checkBoxcbox_5UnSelected();
        }
            break;
        case "cbox_6"_hash:
        {
            this->checkBoxcbox_6UnSelected();
        }
            break;
        default:
            break;
        }
}
void UIIspice::checkBoxcbox_1Selected()
{

}
void UIIspice::checkBoxcbox_1UnSelected()
{

}
void UIIspice::checkBoxcbox_2Selected()
{

}
void UIIspice::checkBoxcbox_2UnSelected()
{

}
void UIIspice::checkBoxcbox_3Selected()
{

}
void UIIspice::checkBoxcbox_3UnSelected()
{

}
void UIIspice::checkBoxcbox_4Selected()
{

}
void UIIspice::checkBoxcbox_4UnSelected()
{

}
void UIIspice::checkBoxcbox_5Selected()
{

}
void UIIspice::checkBoxcbox_5UnSelected()
{

}
void UIIspice::checkBoxcbox_6Selected()
{

}
void UIIspice::checkBoxcbox_6UnSelected()
{

}
//TextField
void UIIspice::textFieldTextField_nameTouchEvent(Ref* obj, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_TextField_nameStrValue = m_TextField_nameTextInput->getStringValue();//输入框中的值

        }
            break;
        case TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
void UIIspice::textFieldTextField_v1TouchEvent(Ref* obj, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_TextField_v1StrValue = m_TextField_v1TextInput->getStringValue();//输入框中的值

        }
            break;
        case TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
void UIIspice::textFieldTextField_v2TouchEvent(Ref* obj, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_TextField_v2StrValue = m_TextField_v2TextInput->getStringValue();//输入框中的值

        }
            break;
        case TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
void UIIspice::textFieldTextField_v3TouchEvent(Ref* obj, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_TextField_v3StrValue = m_TextField_v3TextInput->getStringValue();//输入框中的值

        }
            break;
        case TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
void UIIspice::textFieldTextField_v4TouchEvent(Ref* obj, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_TextField_v4StrValue = m_TextField_v4TextInput->getStringValue();//输入框中的值

        }
            break;
        case TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
void UIIspice::textFieldTextField_v5TouchEvent(Ref* obj, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_TextField_v5StrValue = m_TextField_v5TextInput->getStringValue();//输入框中的值

        }
            break;
        case TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
void UIIspice::textFieldTextField_v6TouchEvent(Ref* obj, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_TextField_v6StrValue = m_TextField_v6TextInput->getStringValue();//输入框中的值

        }
            break;
        case TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
void UIIspice::textFieldTextField_v7TouchEvent(Ref* obj, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_TextField_v7StrValue = m_TextField_v7TextInput->getStringValue();//输入框中的值

        }
            break;
        case TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
void UIIspice::textFieldTextField_v8TouchEvent(Ref* obj, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_TextField_v8StrValue = m_TextField_v8TextInput->getStringValue();//输入框中的值

        }
            break;
        case TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
void UIIspice::textFieldTextField_v9TouchEvent(Ref* obj, TextField::EventType type)
{
    switch (type) {
        case TextField::EventType::ATTACH_WITH_IME://开启输入
        {
            
        }
            break;
        case TextField::EventType::DETACH_WITH_IME://关闭输入
        {
            m_TextField_v9StrValue = m_TextField_v9TextInput->getStringValue();//输入框中的值

        }
            break;
        case TextField::EventType::INSERT_TEXT://插入新文本
        {
            
        }
            break;
        case TextField::EventType::DELETE_BACKWARD://删除文本
        {
            
        }
            break;
        default:
            break;
    }
}
//Panel
void UIIspice::initLayoutlayer_ring()
{

}
void UIIspice::initLayoutPanel_25()
{

}
void UIIspice::initLayoutlayer_mido()
{

}
void UIIspice::initLayoutlayer_downleft()
{

}
void UIIspice::initLayoutlayer_panbg()
{

}
void UIIspice::initLayoutlayer_wavecell()
{

}
void UIIspice::initLayoutlayer_devicecell()
{

}
void UIIspice::initLayoutlayer_cmdcell()
{

}
void UIIspice::initLayoutlayer_setcell()
{

}
void UIIspice::initLayoutlayer_downleft_set()
{

}
void UIIspice::initLayoutlayer_opencell()
{

}
//ListView
//Panel_14+listview_wave,Panel+ListView
void UIIspice::initListViewlistview_wave()
{
            m_listlistview_waveDataVect.clear();
}
void UIIspice::setListViewlistview_waveItem(Widget* w,int n)//初始化子控件
{

}
void UIIspice::listScrollViewlistview_waveEvent(Ref *pSender, ui::ScrollView::EventType type)//滚动图层回调函数,控制滑块
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLLING:        //滚动过程中有好多次响应；
        {
//             Point  pt=m_scrollView->getInnerContainer()->getPosition();
//             Size  size=m_scrollView->getInnerContainer()->getContentSize();
//             
//             float  percentScroll=(size.height+pt.y)/size.height*70;
//             m_slider->setPercent(percentScroll); 
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:     //到达顶点，只是一次响应；
        {
            //m_slider->setPercent(14);//设置滑动条位置
        }
            break;
            
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:    //到达底点；
        {
            //m_slider->setPercent(86);
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_LEFT: //横向时到达左边
        {
        
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_RIGHT: //横向时到达右边
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_TOP: //顶部回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_BOTTOM: //底部回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_LEFT: //左边回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_RIGHT: //右边回弹事件
        {
        
        }
            break;
        default:
            break;
    }
}   
void UIIspice::listview_waveEvent(Ref *pSender, ui::ListView::EventType type)
{
    ListView* plistView = static_cast<ListView*>(pSender);
    if(!plistView){
        log("ScrollView  is  NULL"); 
        return;
    }
    switch (type) {
        case ui::ListView::EventType::ON_SELECTED_ITEM_END :    //点击结束；
        {
            int  n=(int)plistView->getCurSelectedIndex();      //n对应着
            this->setPanellistview_waveItemShow(n);                           // 显示底；
//            if(n!=m_prePress){   //隐藏前一项的底；
//                this->setPanellistview_waveItemHide();  
//            }    
            m_prePresslistview_wave=n;                                 //前一项的索引；
        }
            break;
        case ui::ListView::EventType::ON_SELECTED_ITEM_START :    //点选开始时；
        {
           // ssize_t  n=plistView->getCurSelectedIndex();      //n对应着m_equip;
        }
            break;
        default:
            break;
    }
}
void UIIspice::setPanellistview_waveItemShow(int n)
{
    Layout* tmpItem = dynamic_cast<Layout*>(m_waveListView->getItem(n));
}
void UIIspice::setPanellistview_waveItemHide()
{
    Layout* tmpItem = dynamic_cast<Layout*>(m_waveListView->getItem(m_prePresslistview_wave));
} 
//Panel_14+listview_device,Panel+ListView
void UIIspice::initListViewlistview_device()
{
    m_listlistview_deviceDataVect.clear();
}
void UIIspice::setListViewlistview_deviceItem(Widget* w,int n)//初始化子控件
{

}
void UIIspice::listScrollViewlistview_deviceEvent(Ref *pSender, ui::ScrollView::EventType type)//滚动图层回调函数,控制滑块
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLLING:        //滚动过程中有好多次响应；
        {
//             Point  pt=m_scrollView->getInnerContainer()->getPosition();
//             Size  size=m_scrollView->getInnerContainer()->getContentSize();
//             
//             float  percentScroll=(size.height+pt.y)/size.height*70;
//             m_slider->setPercent(percentScroll); 
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:     //到达顶点，只是一次响应；
        {
            //m_slider->setPercent(14);//设置滑动条位置
        }
            break;
            
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:    //到达底点；
        {
            //m_slider->setPercent(86);
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_LEFT: //横向时到达左边
        {
        
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_RIGHT: //横向时到达右边
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_TOP: //顶部回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_BOTTOM: //底部回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_LEFT: //左边回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_RIGHT: //右边回弹事件
        {
        
        }
            break;
        default:
            break;
    }
}   
void UIIspice::listview_deviceEvent(Ref *pSender, ui::ListView::EventType type)
{
    ListView* plistView = static_cast<ListView*>(pSender);
    if(!plistView){
        log("ScrollView  is  NULL"); 
        return;
    }
    switch (type) {
        case ui::ListView::EventType::ON_SELECTED_ITEM_END :    //点击结束；
        {
            ssize_t  n=plistView->getCurSelectedIndex();      //n对应着
            this->setListView_deviceItemShow(n);                           // 显示底；
//            if(n!=m_prePress){   //隐藏前一项的底；
//                this->setPanellistview_deviceItemHide();  
//            }    
            m_prePresslistview_device=n;                                 //前一项的索引；
        }
            break;
        case ui::ListView::EventType::ON_SELECTED_ITEM_START :    //点选开始时；
        {
            //ssize_t  n=plistView->getCurSelectedIndex();      //n对应着m_equip;
        }
            break;
        default:
            break;
    }
}

void UIIspice::setListView_deviceItemShow(int n)
{
    Layout* tmpItem = dynamic_cast<Layout*>(m_deviceListView->getItem(n));
}
void UIIspice::setPanellistview_deviceItemHide()
{
    Layout* tmpItem = dynamic_cast<Layout*>(m_deviceListView->getItem(m_prePresslistview_device));
} 
//Panel_14+ListView_cmdline,Panel+ListView
void UIIspice::initListViewListView_cmdline()
{
    m_listListView_cmdlineDataVect.clear();
}
void UIIspice::setListViewListView_cmdlineItem(Widget* w,int n)//初始化子控件
{
    
}
void UIIspice::listScrollViewListView_cmdlineEvent(Ref *pSender, ui::ScrollView::EventType type)//滚动图层回调函数,控制滑块
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLLING:        //滚动过程中有好多次响应；
        {
//             Point  pt=m_scrollView->getInnerContainer()->getPosition();
//             Size  size=m_scrollView->getInnerContainer()->getContentSize();
//             
//             float  percentScroll=(size.height+pt.y)/size.height*70;
//             m_slider->setPercent(percentScroll); 
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:     //到达顶点，只是一次响应；
        {
            //m_slider->setPercent(14);//设置滑动条位置
        }
            break;
            
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:    //到达底点；
        {
            //m_slider->setPercent(86);
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_LEFT: //横向时到达左边
        {
        
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_RIGHT: //横向时到达右边
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_TOP: //顶部回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_BOTTOM: //底部回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_LEFT: //左边回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_RIGHT: //右边回弹事件
        {
        
        }
            break;
        default:
            break;
    }
}   
void UIIspice::ListView_cmdlineEvent(Ref *pSender, ui::ListView::EventType type)
{
    ListView* plistView = static_cast<ListView*>(pSender);
    if(!plistView){
        log("ScrollView  is  NULL"); 
        return;
    }
    switch (type) {
        case ui::ListView::EventType::ON_SELECTED_ITEM_END :    //点击结束；
        {
            ssize_t  n=plistView->getCurSelectedIndex();      //n对应着
            this->setPanelListView_cmdlineItemShow(n);                           // 显示底；
//            if(n!=m_prePress){   //隐藏前一项的底；
//                this->setPanelListView_cmdlineItemHide();  
//            }    
            m_prePressListView_cmdline=n;                                 //前一项的索引；
        }
            break;
        case ui::ListView::EventType::ON_SELECTED_ITEM_START :    //点选开始时；
        {
           // ssize_t  n=plistView->getCurSelectedIndex();      //n对应着m_equip;
        }
            break;
        default:
            break;
    }
}
void UIIspice::setPanelListView_cmdlineItemShow(int n)
{
    Layout* tmpItem = dynamic_cast<Layout*>(m_cmdlineListView->getItem(n));
}
void UIIspice::setPanelListView_cmdlineItemHide()
{
    Layout* tmpItem = dynamic_cast<Layout*>(m_cmdlineListView->getItem(m_prePressListView_cmdline));
} 
//Panel_14+ListView_downleft_open,Panel+ListView
void UIIspice::initListViewListView_downleft_open()
{
            m_listListView_downleft_openDataVect.clear();
}
void UIIspice::setListViewListView_downleft_openItem(Widget* w,int n)//初始化子控件
{

}
void UIIspice::listScrollViewListView_downleft_openEvent(Ref *pSender, ui::ScrollView::EventType type)//滚动图层回调函数,控制滑块
{
    switch (type) {
        case ui::ScrollView::EventType::SCROLLING:        //滚动过程中有好多次响应；
        {
//             Point  pt=m_scrollView->getInnerContainer()->getPosition();
//             Size  size=m_scrollView->getInnerContainer()->getContentSize();
//             
//             float  percentScroll=(size.height+pt.y)/size.height*70;
//             m_slider->setPercent(percentScroll); 
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_TOP:     //到达顶点，只是一次响应；
        {
            //m_slider->setPercent(14);//设置滑动条位置
        }
            break;
            
        case ui::ScrollView::EventType::SCROLL_TO_BOTTOM:    //到达底点；
        {
            //m_slider->setPercent(86);
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_LEFT: //横向时到达左边
        {
        
        }
            break;
        case ui::ScrollView::EventType::SCROLL_TO_RIGHT: //横向时到达右边
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_TOP: //顶部回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_BOTTOM: //底部回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_LEFT: //左边回弹事件
        {
        
        }
            break;
        case ui::ScrollView::EventType::BOUNCE_RIGHT: //右边回弹事件
        {
        
        }
            break;
        default:
            break;
    }
}   
void UIIspice::ListView_downleft_openEvent(Ref *pSender, ui::ListView::EventType type)
{
    ListView* plistView = static_cast<ListView*>(pSender);
    if(!plistView){
        log("ScrollView  is  NULL"); 
        return;
    }
    switch (type) {
        case ui::ListView::EventType::ON_SELECTED_ITEM_END :    //点击结束；
        {
            int n=(int)plistView->getCurSelectedIndex();      //n对应着
            this->setPanelListView_downleft_openItemShow(n);                           // 显示底；
//            if(n!=m_prePress){   //隐藏前一项的底；
//                this->setPanelListView_downleft_openItemHide();  
//            }    
            m_prePressListView_downleft_open=n;                                 //前一项的索引；
        }
            break;
        case ui::ListView::EventType::ON_SELECTED_ITEM_START :    //点选开始时；
        {
          //  ssize_t  n=plistView->getCurSelectedIndex();      //n对应着m_equip;
        }
            break;
        default:
            break;
    }
}
void UIIspice::setPanelListView_downleft_openItemShow(int n)
{
    Layout* tmpItem = dynamic_cast<Layout*>(m_downleft_openListView->getItem(n));
}
void UIIspice::setPanelListView_downleft_openItemHide()
{
    Layout* tmpItem = dynamic_cast<Layout*>(m_downleft_openListView->getItem(m_prePressListView_downleft_open));
} 

void UIIspice::onEnter()
{
    Layer::onEnter();
    
}
void UIIspice::onExit()
{

    Layer::onExit();
}
