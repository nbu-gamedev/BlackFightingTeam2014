#include "StateManager.h"
#include "XML_Parser.h"
#include "Input.h"
#include "Game.h"
const std::string MainMenuState::s_menuID = "MENU";
const std::string ChoseCharState::s_chosechar = "CHOSE_CHAR";
const std::string CreateChar::s_create = "CREATE_CHAR";
const std::string PlayState::s_game = "GAME";

void GameStateMachine::pushState(GameState* pState)
{
    m_gameStates.push_back(pState);
    m_gameStates.back()->Enter();
}

void GameStateMachine::popState()
{
    if(!m_gameStates.empty())
    {
        if(m_gameStates.back()->Exit())
        {
            delete m_gameStates.back();
            m_gameStates.pop_back();
        }
    }
}

// TODO : Fix crashing the game after using changeState function.
void GameStateMachine::changeState(GameState* pState)
{
    if(!m_gameStates.empty())
    {
        if(m_gameStates.back()->getState() == pState->getState())
        {
            return;
        }

        if(m_gameStates.back()->Exit())
        {
            delete m_gameStates.back();
            m_gameStates.pop_back();
        }
        m_gameStates.push_back(pState);
        m_gameStates.back()->Enter();
    }
}

void GameStateMachine::Update()
{
    if(!m_gameStates.empty())
    {
        m_gameStates.back()->Update();
    }
}

void GameStateMachine::Draw()
{
    if(!m_gameStates.empty())
    {
        m_gameStates.back()->Draw();
    }
}

/*
    INIT function of the States
*/

bool MainMenuState::Enter()
{
    std::cout << "Enter in Menu State!\n";
    StateParser MenuStateXML;
    MenuStateXML.parseState("data/config.xml","MENU",&m_menuObjects,&m_textures);

    m_callbacks.push_back(0);
    m_callbacks.push_back(std::bind(&MainMenuState::s_menuPlay, this));
    m_callbacks.push_back(std::bind(&MainMenuState::s_menuExit, this));
    setCallbacks(m_callbacks);

    m_loginName = new TextField();
    m_loginName->load(new LoadParam(50,430,300,40,300,40,"",0,1));
    m_password = new TextField();
    m_password->load(new LoadParam(50,475,300,40,300,40,"",0,1));
    m_password->SetPassHide();
    m_menuObjects.push_back(m_loginName);
    m_menuObjects.push_back(m_password);

    return true;
}

bool ChoseCharState::Enter()
{
    std::cout << "Enter ChoseChar state" << std::endl;
    StateParser XMLState;
    XMLState.parseState("data/config.xml",s_chosechar,&m_objects,&m_textures);

    m_callbacks.push_back(0);
    m_callbacks.push_back(std::bind(&ChoseCharState::s_CreateCharWindow,this));
    m_callbacks.push_back(std::bind(&ChoseCharState::s_Exit,this));
    m_callbacks.push_back(std::bind(&ChoseCharState::Play,this));

    setCallbacks(m_callbacks);

    GM::Instance()->load("data/image/UI/character.png","character_set");
    m_textures.push_back("character_set");
    GM::Instance()->load("data/image/icon/orc.png","Orc");
    m_textures.push_back("Orc");
    GM::Instance()->load("data/image/icon/elf.png","Elf");
    m_textures.push_back("Elf");

    m_orc.load(new LoadParam(0,100,1242,3208,200,520,"orc_char",0,1));
    m_elf.load(new LoadParam(0,100,1242,3208,200,520,"elf_char",0,1));

    for(unsigned int i = 0; i < 5;i++)
    {
        CharacterButton* tmp = new CharacterButton();
        tmp->load(new LoadParam(0,i*105,200,100,200,100,"character_set",0,2));
        if(ThePlayer.m_race[i] > 0)
        {
            tmp->setName_Race(ThePlayer.m_CharNames[i],ThePlayer.m_race[i]);
        }
        m_characters.push_back(tmp);
    }
    m_Charactive = -1;
    return true;
}


bool CreateChar::Enter()
{
    std::cout << "Enter Create state" << std::endl;
    StateParser XMLState;
    XMLState.parseState("data/config.xml",s_create,&m_objects,&m_textures);

    m_callbacks.push_back(0);
    m_callbacks.push_back(std::bind(&CreateChar::s_CreateCharacter,this));
    m_callbacks.push_back(std::bind(&CreateChar::s_Back,this));
    setCallbacks(m_callbacks);

    for(unsigned int i = 0; i < 2;i++)
    {
        CharacterButton* tmp = new CharacterButton();
        tmp->load(new LoadParam(0,i*105,200,100,200,100,"character_set",0,2));
        tmp->setName_Race("",i+1);
        m_characters.push_back(tmp);
    }
    m_orc.load(new LoadParam(0,100,1242,3208,200,520,"orc_char",0,1));
    m_elf.load(new LoadParam(0,100,1242,3208,200,520,"elf_char",0,1));

    m_CharName.load(new LoadParam(300,650,200,40,200,40,"",0,1));
    m_CharName.setFontSize(20);
    m_Picked = -1;
    return true;
}

bool PlayState::Enter()
{
    std::cout << "Enter Game State !\n";
    StateParser XMLState;
    XMLState.parseState("data/config.xml",s_game,&m_objects,&m_textures);

    m_callbacks.push_back(0);
    m_callbacks.push_back(std::bind(&PlayState::ExitGame, this));

    return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

/*
    EXIT function of the States for destroying objects from the state free some memory
*/
bool MainMenuState::Exit()
{
    for(unsigned int i = 0;i < m_menuObjects.size();i++)
    {
        m_menuObjects[i]->Clean();
    }
    m_menuObjects.clear();

    for(unsigned int i = 0;i < m_textures.size();i++)
    {
        GM::Instance()->clearTextureFromMap(m_textures[i]);
    }
    m_textures.clear();
    std::cout << "Leaving Menu state!\n";
    return true;
}

bool ChoseCharState::Exit()
{
    for(unsigned int i = 0;i < m_objects.size();i++)
    {
        m_objects[i]->Clean();
    }
    m_objects.clear();
    for(unsigned int i = 0;i < m_textures.size();i++)
    {
        GM::Instance()->clearTextureFromMap(m_textures[i]);
    }
    m_textures.clear();
    return true;
}

bool CreateChar::Exit()
{
    std::cout << "Leaving Create State\n";
    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        m_objects[i]->Clean();
    }
    m_objects.clear();
    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        GM::Instance()->clearTextureFromMap(m_textures[i]);
    }
    m_textures.clear();
    return true;
}

bool PlayState::Exit()
{
    std::cout << "Leaving Game State\n";
    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        m_objects[i]->Clean();
    }
    m_objects.clear();
    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        GM::Instance()->clearTextureFromMap(m_textures[i]);
    }
    m_textures.clear();
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////

/*
    CALLBACKS Setting functions for the buttons
*/
void MainMenuState::setCallbacks(const std::vector<FunctionName>& callbacks)
{
    for(unsigned int i = 0; i < m_menuObjects.size();i++)
    {
        if( dynamic_cast<MenuButton*>(m_menuObjects[i]))
        {
            MenuButton* pButton = dynamic_cast<MenuButton*>(m_menuObjects[i]);
            std::cout << pButton->getCallback() << std::endl;
            pButton->setCallBack(callbacks[pButton->getCallback()]);
        }
    }
}

void ChoseCharState::setCallbacks(const std:: vector<FunctionName>& callbacks)
{
    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        if( dynamic_cast<MenuButton*>(m_objects[i]))
        {
            MenuButton* pButton = dynamic_cast<MenuButton*>(m_objects[i]);
            pButton->setCallBack(callbacks[pButton->getCallback()]);
        }
    }
}

void CreateChar::setCallbacks(const std::vector<FunctionName>& callbacks)
{
    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        if( dynamic_cast<MenuButton*>(m_objects[i]))
        {
            MenuButton* pButton = dynamic_cast<MenuButton*>(m_objects[i]);
            pButton->setCallBack(callbacks[pButton->getCallback()]);
        }
    }
}

void PlayState::setCallbacks(const std::vector<FunctionName>& callbacks)
{
    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        if( dynamic_cast<MenuButton*>(m_objects[i]))
        {
            MenuButton* pButton = dynamic_cast<MenuButton*>(m_objects[i]);
            pButton->setCallBack(callbacks[pButton->getCallback()]);
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////

/*
    UPDATE Functions for the objects in the States
*/
void MainMenuState::Update()
{
    for(unsigned int i = 0;i < m_menuObjects.size();i++)
    {
        m_menuObjects[i]->Update();
    }
    int isMess = ComCon::Instance()->receiveData(m_server_message);
    if(isMess == 0)
    {
        if(m_server_message.mID == eMessID_autentOk)
        {

        }
        else if(m_server_message.mID == eMessID_charList)
        {
            char tmpName[1024];
            ThePlayer.m_race[ThePlayer.m_indx] = m_server_message.getVarU16();
            strcpy(tmpName,(char*)&(m_server_message.data[2]));
            ThePlayer.m_CharNames[ThePlayer.m_indx] = tmpName;
            ThePlayer.m_indx++;
            std::cout <<  ThePlayer.m_CharNames[ThePlayer.m_indx-1] << " " <<  ThePlayer.m_race[ThePlayer.m_indx-1] <<std::endl;
        }
        if(m_server_message.mID == eMessID_done)
        {
            G::Instance()->getStateMachine()->pushState(new ChoseCharState());
        }
    }
}

void ChoseCharState::Update()
{
    if(ThePlayer.m_update)
    {
        m_characters[ThePlayer.m_indx-1]->setName_Race(ThePlayer.m_CharNames[ThePlayer.m_indx-1],ThePlayer.m_race[ThePlayer.m_indx-1]);
        ThePlayer.m_update = false;
    }

    if(m_Charactive < 0)
    {
         for(unsigned int i = 0; i < 5;i++)
        {
            m_characters[i]->Update();
            if(m_characters[i]->isActive())
            {
                m_Charactive = i;
                break;
            }
        }
    }
    else
    {
        m_characters[m_Charactive]->Update();
        if(!m_characters[m_Charactive]->isActive())
        {
            m_Charactive = -1;
        }
    }

    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        m_objects[i]->Update();
    }
}
void CreateChar::Update()
{
    if(m_Picked < 0)
    {
         for(unsigned int i = 0; i < 2;i++)
        {
            m_characters[i]->Update();
            if(m_characters[i]->isActive())
            {
                m_Picked = i;
                break;
            }
        }
    }
    else
    {
        m_characters[m_Picked]->Update();
        if(!m_characters[m_Picked]->isActive())
        {
            m_Picked = -1;
        }
    }
    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        m_objects[i]->Update();
    }
    m_CharName.Update();
}

void PlayState::Update()
{
    for(unsigned int i = 0;i < m_objects.size();i++)
    {
        m_objects[i]->Draw();
    }
}
//////////////////////////////////////////////////////

/*
    DRAW functions
*/

void MainMenuState::Draw()
{
    for(unsigned int i = 0;i < m_menuObjects.size();i++)
    {
        m_menuObjects[i]->Draw();
    }
}

void ChoseCharState::Draw()
{
    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        m_objects[i]->Draw();
    }

    for(unsigned int i = 0; i < 5;i++)
    {
        m_characters[i]->Draw();
    }
    if(m_Charactive >=0)
    {
        if(m_characters[m_Charactive]->getRace() == "Orc")
        {
            m_orc.Draw();
        }
        if(m_characters[m_Charactive]->getRace() == "Elf")
        {
            m_elf.Draw();
        }
    }
}

void CreateChar::Draw()
{
    for(unsigned int i = 0; i < m_objects.size();i++)
    {
        m_objects[i]->Draw();
    }

    for(unsigned int i = 0; i < 2;i++)
    {
        m_characters[i]->Draw();
    }

    if(m_Picked >=0)
    {
        if(m_characters[m_Picked]->getRace() == "Orc")
        {
            m_orc.Draw();
        }
        if(m_characters[m_Picked]->getRace() == "Elf")
        {
            m_elf.Draw();
        }
    }
    m_CharName.Draw();
}

void PlayState::Draw()
{
    for(unsigned int i = 0;i < m_objects.size();i++)
    {
        m_objects[i]->Draw();
    }
}

////////////////////////////////////////////////////////////////////////

/*
    Button functions
*/
void MainMenuState::s_menuExit()
{
    std::cout << "Clicked Exit Button\n";
    G::Instance()->Quit();
}

void MainMenuState::s_menuPlay()
{
    std::cout << "Clicked Play Button\n";
    if(m_loginName->getText().length()>0 && m_password->getText().length()>0)
    {
        m_server_message.mID = eMessID_uName;
        m_server_message.m_dataSize = m_loginName->getText().length()+1;
        memcpy(m_server_message.data,m_loginName->getText().c_str(),m_loginName->getText().length()+1);
        ComCon::Instance()->sendData(m_server_message);

        m_server_message.mID = eMessID_pass;
        m_server_message.m_dataSize = m_password->getText().length()+1;
        memcpy(m_server_message.data,m_password->getText().c_str(),m_password->getText().length()+1);
        ComCon::Instance()->sendData(m_server_message);
    }
}

void ChoseCharState::s_CreateCharWindow()
{
    G::Instance()->getStateMachine()->pushState(new CreateChar());
}

void ChoseCharState::s_Exit()
{
    G::Instance()->Quit();
}

void ChoseCharState::Play()
{
    std::cout << "PLAY\n";
    if(m_Charactive >=0)
    {
        m_characters[m_Charactive]->getName();
        m_server_message.mID = eMessID_choosChar;
        m_server_message.m_dataSize = m_characters[m_Charactive]->getName().length()+1;
        memcpy(m_server_message.data,m_characters[m_Charactive]->getName().c_str(),m_server_message.m_dataSize);
        std::cout << m_characters[m_Charactive]->getName()<<std::endl;
        ComCon::Instance()->sendData(m_server_message);
    }
}

void CreateChar::s_CreateCharacter()
{
    if(m_Picked >=0 && m_CharName.getText().length()>0)
    {
         if(ThePlayer.m_indx <4)
        {
            if(m_characters[m_Picked]->getRace() == "Orc")
            {
                ThePlayer.m_race[ThePlayer.m_indx] = 2;
                ThePlayer.m_CharNames[ThePlayer.m_indx] = m_CharName.getText();
            }
            else
            {
                ThePlayer.m_race[ThePlayer.m_indx] = 1;
                ThePlayer.m_CharNames[ThePlayer.m_indx] = m_CharName.getText();
            }
            m_server_message.mID = eMessID_crateChar;
            m_server_message.data[0] = static_cast<unsigned char>( ThePlayer.m_race[ThePlayer.m_indx] >> 8);
            m_server_message.data[1] = static_cast<unsigned char>( ThePlayer.m_race[ThePlayer.m_indx]);
            m_server_message.m_dataSize = m_CharName.getText().length()+3;
            memcpy(&(m_server_message.data[2]),m_CharName.getText().c_str(),m_CharName.getText().length()+3);
            ThePlayer.m_indx++;
            ComCon::Instance()->sendData(m_server_message);
            ThePlayer.m_update = true;
            G::Instance()->getStateMachine()->popState();
        }
        SDL_Delay(200);
    }
}

void CreateChar::s_Back()
{
    G::Instance()->getStateMachine()->popState();
    SDL_Delay(200);
}

void PlayState::ExitGame()
{
    std::cout << "Exiting the game!\n";
}
/////////////////////////////////////////////////////////////////////////////





