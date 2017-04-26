#include "Tests.h"

void Tests::batchedSFMLRendering(sf::Texture& texture)
{
    liquid::graphics::Renderer* renderer = liquid::common::GameManager::instance().getRendererClass();
    liquid::common::GameScene* scene = liquid::common::GameManager::instance().peekGameSceneFront();

    liquid::impl::SFMLRenderableBatch* batch = new liquid::impl::SFMLRenderableBatch(texture, 3);
    liquid::common::Entity* entity1 = new liquid::common::Entity();
    liquid::common::Entity* entity2 = new liquid::common::Entity();
    liquid::common::Entity* entity3 = new liquid::common::Entity();

    entity1->setPosition(340.0f, 340.0f);
    entity2->setPosition(475.0f, 175.0f);
    entity3->setPosition(800.0f, 350.0f);

    scene->addEntity(entity1);
    scene->addEntity(entity2);
    scene->addEntity(entity3);

    entity1->setVerticesPtr(batch->nextVertices());
    entity2->setVerticesPtr(batch->nextVertices());
    entity3->setVerticesPtr(batch->nextVertices());
    renderer->addRenderable(batch);

    mEntity = entity1;

    auto leftHandle = liquid::events::EventDispatcher<liquid::events::KeyboardEventData>::addListener(
        [&ent = mEntity](const liquid::events::KeyboardEventData& evnt)->bool
    {
        if (evnt.mKeyCode == sf::Keyboard::A)
            ent->addPosition(-5.0f, 0.0f);
        else if (evnt.mKeyCode == sf::Keyboard::D)
            ent->addPosition(5.0f, 0.0f);
        else if (evnt.mKeyCode == sf::Keyboard::W)
            ent->addPosition(0.0f, -5.0f);
        else if (evnt.mKeyCode == sf::Keyboard::S)
            ent->addPosition(0.0f, 5.0f);
        else if (evnt.mKeyCode == sf::Keyboard::Escape)
            liquid::common::GameManager::instance().setGameRunning(false);

        return true;
    });
}

void Tests::particles(sf::Texture& texture)
{
    liquid::graphics::Renderer* renderer = liquid::common::GameManager::instance().getRendererClass();
    liquid::common::GameScene* scene = liquid::common::GameManager::instance().peekGameSceneFront();

    liquid::parser::ParserConfig particleParser;
    particleParser.parseString(liquid::data::ParticleData::mDefaultParticle);

    liquid::impl::SFMLRenderableBatch* particleBatch = new liquid::impl::SFMLRenderableBatch(texture, 500);
    liquid::data::ParticleData* particleData = new liquid::data::ParticleData(particleParser);
    liquid::common::ParticleEmitter* emitter = new liquid::common::ParticleEmitter(*particleData, particleBatch, 500);
    emitter->setPosition(950.0f, 500.0f);
    emitter->setRepeat(true);
    particleBatch->setBlendMode(sf::BlendAdd);
    renderer->addRenderable(particleBatch);
    scene->addEntity(emitter);
    mEmitter = emitter;

    auto mouseHandle = liquid::events::EventDispatcher<liquid::events::MouseEventData>::addListener(
        [&em = mEmitter](const liquid::events::MouseEventData& evnt)->bool
    {
        if (evnt.mMouseButton == sf::Mouse::Left && evnt.mPressed == true)
            em->emit();

        return true;
    }
    );
}

void Tests::events()
{
    liquid::graphics::Renderer* renderer = liquid::common::GameManager::instance().getRendererClass();
    liquid::common::GameScene* scene = liquid::common::GameManager::instance().peekGameSceneFront();

    using namespace liquid::events;
    auto handle1 = EventDispatcher<KeyboardEventData>::
        addListener([](const KeyboardEventData& evnt)->bool
    {
        std::cout << "Keyboard Event Listener 1: " << evnt.mKeyCode << "\n";
        return true;
    });

    auto handle2 = EventDispatcher<KeyboardEventData>::
        addListener([](const KeyboardEventData& evnt)->bool
    {
        std::cout << "Keyboard Event Listener 2: " << evnt.mKeyCode << "\n";
        return true;
    });

    auto handle3 = EventDispatcher<WindowEventData>::
        addListener([](const WindowEventData& evnt)->bool
    {
        std::cout << "Window Event Listener 1:" << evnt.mEventType << "\n";
        return true;
    });

    std::cout << "Keyboard Event 1 ID: " << handle1 << std::endl;
    std::cout << "Keyboard Event 2 ID: " << handle2 << std::endl;
    std::cout << "Window Event 1 ID: " << handle3 << "\n\n";

    KeyboardEventData data1(32, true);
    KeyboardEventData data2(-1, true);
    WindowEventData data3(WindowEventData::eWindowEventType::WINDOWEVENT_CLOSED);

    EventDispatcher<KeyboardEventData>::triggerListeners(data1);
    EventDispatcher<KeyboardEventData>::triggerListeners(data2);
    EventDispatcher<WindowEventData>::triggerListeners(data3);

    std::cout << "\nNow removing second listener\n";
    EventDispatcher<KeyboardEventData>::removeListener(handle2);
    EventDispatcher<KeyboardEventData>::triggerListeners(data1);
    EventDispatcher<KeyboardEventData>::triggerListeners(data2);

    using namespace liquid::common;
    Entity* entity = new Entity();
    entity->setEntityUID("TestEntity");
    entity->setPosition(450.0f, 320.0f);
    int32_t resID = ResourceManager<Entity>::addResource(entity);

    KeyboardEventData kData(23, true);
    int32_t resID2 = ResourceManager<KeyboardEventData>::addResource(&kData);

    std::cout << "\nEntity Resource ID: " << resID << std::endl;
    std::cout << "Keyboard Data Resource ID: " << resID2 << "\n\n";

    Entity* retrieved = ResourceManager<Entity>::getResource(resID);
    std::cout << "\nENTITY DATA\nUID of " << retrieved->getEntityUID() << "\n";
    std::cout << "Position of " << retrieved->getPositionX() << ",";
    std::cout << retrieved->getPositionY() << "\n\n";

    using namespace liquid::utilities;
    int r1 = Random::instance().randomRange(5, 20);
    float r2 = Random::instance().randomRange(0.5f, 4.92f);
    std::array<float, 2> r3 = Random::instance().randomRange({ 5.2f,10.0f }, { 4.0f, 6.35f });
    std::array<float, 2> r4 = Random::instance().randomUnitVector();

    std::cout << "rand1 : " << r1 << std::endl;
    std::cout << "rand2 : " << r2 << std::endl;
    std::cout << "rand3 : " << r3.at(0) << ", " << r3.at(1) << std::endl;
    std::cout << "rand4 : " << r4.at(0) << ", " << r4.at(1) << "\n\n";

    std::string test;
    std::cin >> test;
    std::cout << "end";
}

void Tests::navigation()
{
    liquid::navigation::NavGraph* graph = new liquid::navigation::NavGraph();
    graph->addNavNode(500.0f, 500.0f);
    graph->addNavNode(250.0f, 250.0f);
    graph->addNavNode(700.0f, 250.0f);
    graph->addNavNode(710.0f, 250.0f);
    graph->addNavNode(720.0f, 250.0f);
    graph->addNavNode(730.0f, 250.0f);

    graph->addNavEdge(0, 1);
    graph->addNavEdge(0, 2);
    graph->addNavEdge(0, 3);
    graph->addNavEdge(0, 4);
    graph->addNavEdge(0, 5);

    liquid::navigation::AStar* aStar = new liquid::navigation::AStar(graph);
    liquid::navigation::NavPath path = aStar->search(1, 4);

    while (path.isEmpty() == false)
        std::cout << "node : " << path.popFront().getNodeIndex() << std::endl;

    std::cout << std::endl;
}

void Tests::parserXML()
{
    liquid::parser::ParserXML* xmlParser = new liquid::parser::ParserXML;
    xmlParser->parseFile("atlas.xml");
    xmlParser->dumpXMLTreeToFile();

    liquid::parser::ParserXML atlasParser;
    atlasParser.parseFile("atlas.xml");
    liquid::data::TextureAtlas* atlas = new liquid::data::TextureAtlas(atlasParser);
    atlas->compile();

    liquid::parser::ParserNodeSearch nodeSearch(atlasParser.getRootParserNode());
    std::list<liquid::parser::ParserNode*> names = nodeSearch.findParserNodes("name");

    nodeSearch.traverseChild("region");
    nodeSearch.traverseChild("name");

    liquid::parser::ParserNode* curNode = nodeSearch.getCurrentNode();
}

void Tests::quadTree()
{
    std::vector<liquid::common::Entity*> entities;
    entities.resize(150);
    for (uint32_t i = 0; i < 150; i++)
    {
        float posX = liquid::utilities::Random::instance().randomRange(100.0f, 500.0f);
        float posY = liquid::utilities::Random::instance().randomRange(100.0f, 500.0f);

        entities[i] = new liquid::common::Entity();
        entities[i]->setPosition(posX, posY);
    }

    liquid::spatial::QuadTree* quadTree =
        new liquid::spatial::QuadTree(4, { 300.0f,300.0f }, { 600.0f, 600.0f });

    for (int32_t i = 0; i < 150; i++)
        quadTree->insertEntity(entities[i]);

    for (int32_t i = 0; i < 20; i++)
        quadTree->removeEntity(entities[i]);

    quadTree->pruneDeadBranches();
    std::vector<liquid::common::Entity*> query = quadTree->query({ 0.0f, 0.0f, 600.0f, 600.0f });
}