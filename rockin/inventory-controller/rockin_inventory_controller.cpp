#include <chrono>
#include <iomanip>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <config/yaml.h>
#include <utils/system/argparser.h>

#include <protobuf_comm/client.h>
#include <msgs/BenchmarkState.pb.h>
#include <msgs/BenchmarkFeedback.pb.h>
#include <msgs/Inventory.pb.h>

#include <gtkmm.h>
#include <pangomm.h>
#include <glibmm.h>

protobuf_comm::ProtobufStreamClient client;
std::string host;
int port;
Glib::RefPtr<Gtk::Builder> builder;
std::chrono::time_point<std::chrono::system_clock> last_gui_update;
boost::mutex mutex;
std::shared_ptr<rockin_msgs::BenchmarkState> benchmark_state;
std::shared_ptr<rockin_msgs::Inventory> inventory;
std::shared_ptr<rockin_msgs::Inventory> inventory_old;
std::shared_ptr<rockin_msgs::Inventory> inventory_preview;


void handle_message(uint16_t comp_id, uint16_t msg_type,
      std::shared_ptr<google::protobuf::Message> msg)
{
  // Prevent simultaneous access to the refbox state from gui and network
  boost::mutex::scoped_lock lock(mutex);

  if (std::dynamic_pointer_cast<rockin_msgs::BenchmarkState>(msg)) {
    benchmark_state = std::dynamic_pointer_cast<rockin_msgs::BenchmarkState>(msg);
  }

  if (std::dynamic_pointer_cast<rockin_msgs::Inventory>(msg)) {
    inventory = std::dynamic_pointer_cast<rockin_msgs::Inventory>(msg);
  }
}


bool idle_handler() {
  if ((std::chrono::system_clock::now() - last_gui_update) < std::chrono::milliseconds(100)) {
    usleep(10000);
    return true;
  }
  last_gui_update = std::chrono::system_clock::now();


  // Prevent simultaneous access to the cfh state from gui and network
  boost::mutex::scoped_lock lock(mutex);

  if (benchmark_state) {
    Gtk::Button *button_add_item_to_inventory = 0;
    Gtk::Button *button_remove_item_from_inventory = 0;
    Gtk::Button *button_preview_inventory = 0;
    Gtk::Button *button_update_inventory = 0;
    Gtk::Button *button_discard_changes_to_inventory = 0;
    builder->get_widget("button_add_item_to_inventory", button_add_item_to_inventory);
    builder->get_widget("button_remove_item_from_inventory", button_remove_item_from_inventory);
    builder->get_widget("button_preview_inventory", button_preview_inventory);
    builder->get_widget("button_update_inventory", button_update_inventory);
    builder->get_widget("button_discard_changes_to_inventory", button_discard_changes_to_inventory);

    switch (benchmark_state->state()) {
      case rockin_msgs::BenchmarkState::INIT:
//        button_start->set_sensitive(false);
//        button_pause->set_sensitive(false);
//        button_stop->set_sensitive(false);
      break;

      case rockin_msgs::BenchmarkState::STOPPED:
//        button_start->set_sensitive(true);
//        button_pause->set_sensitive(false);
//        button_stop->set_sensitive(false);
      break;

      case rockin_msgs::BenchmarkState::RUNNING:
//        button_start->set_sensitive(false);
//        button_pause->set_sensitive(true);
//        button_stop->set_sensitive(true);
      break;

      case rockin_msgs::BenchmarkState::PAUSED:
//        button_start->set_sensitive(true);
//        button_pause->set_sensitive(false);
//        button_stop->set_sensitive(false);
      break;

      case rockin_msgs::BenchmarkState::FINISHED:
//        button_start->set_sensitive(false);
//        button_pause->set_sensitive(false);
//        button_stop->set_sensitive(false);
      break;
    }

    // Only activate in FBM2 during STOPPED or FINISHED state
    if ((benchmark_state->phase().type() == rockin_msgs::BenchmarkPhase::FBM)
        && (benchmark_state->phase().type_id() == 2)
        && ((benchmark_state->state() == rockin_msgs::BenchmarkState::STOPPED)
         || benchmark_state->state() == rockin_msgs::BenchmarkState::FINISHED)) {
//      button_success->set_sensitive(true);
//      button_fail->set_sensitive(true);
    } else {
//      button_success->set_sensitive(false);
//      button_fail->set_sensitive(false);
    }
  }

  return true;
}


void handle_disconnect(const boost::system::error_code &error)
{
  usleep(100000);
  client.async_connect(host.c_str(), port);
}


void on_add_item_to_inventory_click()
{
  if (!client.connected()) return;

//  rockin_msgs::SetBenchmarkTransitionEvent cmd_event;
//  cmd_event.set_event(rockin_msgs::SetBenchmarkTransitionEvent::START);
//  client.send(cmd_event);
}


void on_remove_item_from_inventory_click()
{
  if (!client.connected()) return;

//  rockin_msgs::SetBenchmarkTransitionEvent cmd_event;
//  cmd_event.set_event(rockin_msgs::SetBenchmarkTransitionEvent::PAUSE);
//  client.send(cmd_event);
}


void on_preview_inventory_click()
{
  if (!client.connected()) return;

//  rockin_msgs::SetBenchmarkTransitionEvent cmd_event;
//  cmd_event.set_event(rockin_msgs::SetBenchmarkTransitionEvent::STOP);
//  client.send(cmd_event);
}

void on_update_inventory_click()
{
  if (!client.connected()) return;

//  rockin_msgs::BenchmarkFeedback msg;
//  msg.set_grasp_notification(true);
//  client.send(msg);
}


void on_discard_changes_to_inventory_click()
{
  if (!client.connected()) return;

//  rockin_msgs::BenchmarkFeedback msg;
//  msg.set_grasp_notification(false);
//  client.send(msg);
}

void on_choose_item_arena_inventory_changed()
{
  if (!client.connected()) return;

  Gtk::ComboBoxText *comboBoxText_choose_item_arena_inventory = 0;
  builder->get_widget("comboBoxText_choose_item_arena_inventory", comboBoxText_choose_item_arena_inventory);
  std::string phase = comboBoxText_choose_item_arena_inventory->get_active_text();

  rockin_msgs::SetBenchmarkPhase cmd_phase;
//  if (phase == "FBM1") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::FBM);
//    cmd_phase.mutable_phase()->set_type_id(1);
//  } else if (phase == "FBM2") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::FBM);
//    cmd_phase.mutable_phase()->set_type_id(2);
//  } else if (phase == "TBM1") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::TBM);
//    cmd_phase.mutable_phase()->set_type_id(1);
//  } else if (phase == "TBM2") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::TBM);
//    cmd_phase.mutable_phase()->set_type_id(2);
//  } else if (phase == "TBM3") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::TBM);
//    cmd_phase.mutable_phase()->set_type_id(3);
//  } else if (phase == "None") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::NONE);
//    cmd_phase.mutable_phase()->set_type_id(0);
//  }
  client.send(cmd_phase);


  rockin_msgs::SetBenchmarkTransitionEvent cmd_event;
  cmd_event.set_event(rockin_msgs::SetBenchmarkTransitionEvent::RESET);
  client.send(cmd_event);
}

void on_choose_item_complete_inventory_changed()
{
  if (!client.connected()) return;

  Gtk::ComboBoxText *comboBoxText_choose_item_complete_inventory = 0;
  builder->get_widget("comboBoxText_choose_item_complete_inventory", comboBoxText_choose_item_complete_inventory);
  std::string phase = comboBoxText_choose_item_complete_inventory->get_active_text();

  rockin_msgs::SetBenchmarkPhase cmd_phase;
//  if (phase == "FBM1") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::FBM);
//    cmd_phase.mutable_phase()->set_type_id(1);
//  } else if (phase == "FBM2") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::FBM);
//    cmd_phase.mutable_phase()->set_type_id(2);
//  } else if (phase == "TBM1") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::TBM);
//    cmd_phase.mutable_phase()->set_type_id(1);
//  } else if (phase == "TBM2") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::TBM);
//    cmd_phase.mutable_phase()->set_type_id(2);
//  } else if (phase == "TBM3") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::TBM);
//    cmd_phase.mutable_phase()->set_type_id(3);
//  } else if (phase == "None") {
//    cmd_phase.mutable_phase()->set_type(rockin_msgs::BenchmarkPhase::NONE);
//    cmd_phase.mutable_phase()->set_type_id(0);
//  }
  client.send(cmd_phase);


  rockin_msgs::SetBenchmarkTransitionEvent cmd_event;
  cmd_event.set_event(rockin_msgs::SetBenchmarkTransitionEvent::RESET);
  client.send(cmd_event);
}

void initialize_complete_inventory_comboBox()
{
	//if (!client.connected()) return;

	  Gtk::ComboBoxText *comboBoxText_choose_item_complete_inventory = 0;
	  builder->get_widget("comboBoxText_choose_item_complete_inventory", comboBoxText_choose_item_complete_inventory);

	  // Add AX-Parts
	  comboBoxText_choose_item_complete_inventory->append("AX-01 - Bearing Box Type A");
	  comboBoxText_choose_item_complete_inventory->append("AX-02 - Bearing");
	  comboBoxText_choose_item_complete_inventory->append("AX-03 - Axis");
	  comboBoxText_choose_item_complete_inventory->append("AX-04 - Shaft Nut");
	  comboBoxText_choose_item_complete_inventory->append("AX-05 - Distance Tube");
	  comboBoxText_choose_item_complete_inventory->append("AX-06 - Cover Plate");
	  comboBoxText_choose_item_complete_inventory->append("AX-07 - Cover Plate Machined");
	  comboBoxText_choose_item_complete_inventory->append("AX-08 - Pre Assembled Bearing Box");
	  comboBoxText_choose_item_complete_inventory->append("AX-09 - Motor with Gearbox and Encoder");
	  comboBoxText_choose_item_complete_inventory->append("AX-10 - Screw Pack 1 (mounting cover plate to bearing box)");
	  comboBoxText_choose_item_complete_inventory->append("AX-11 - Screw Pack 2 (mounting cover plate to motor");
	  //comboBoxText_choose_item_complete_inventory->append("AX-12");
	  //comboBoxText_choose_item_complete_inventory->append("AX-13");
	  //comboBoxText_choose_item_complete_inventory->append("AX-14");
	  //comboBoxText_choose_item_complete_inventory->append("AX-15");
	  comboBoxText_choose_item_complete_inventory->append("AX-16 - Bearing Box Type B");

	  // Add EM-parts
	  comboBoxText_choose_item_complete_inventory->append("EM-01 - Aid Tray");
	  comboBoxText_choose_item_complete_inventory->append("EM-02 - File Card Box");
	  comboBoxText_choose_item_complete_inventory->append("EM-03 - Box for Fill a Box");
	  //comboBoxText_choose_item_complete_inventory->append("EM-04");
	  //comboBoxText_choose_item_complete_inventory->append("EM-05");
	  comboBoxText_choose_item_complete_inventory->append("EM-06 - Cone Sink Driller");

	  // Add ER-parts
	  comboBoxText_choose_item_complete_inventory->append("ER-01 - Foam Container");
	  comboBoxText_choose_item_complete_inventory->append("ER-02 - Common Shelf Container");
	  comboBoxText_choose_item_complete_inventory->append("ER-03 - Aid Tray Rack");
}

void initialize_arena_inventory_comboBox()
{
	if (!client.connected()) return;

//	if (inventory) {
//	    Gtk::Label *label_inventory = 0;
//	    builder->get_widget("label_inventory", label_inventory);
//
//	    std::stringstream sstr;
//	    for (int i = 0; i < inventory->items_size(); i++) {
//	      const rockin_msgs::Inventory::Item &item = inventory->items(i);
//	      sstr << item.object().description() << ": ";
//
//	      if (inventory->items(i).has_container()) sstr << item.container().description();
//	      else if (inventory->items(i).has_location()) sstr << item.location().description();
//
//	      sstr << std::endl;
//	    }
//	    label_inventory->set_text(sstr.str());

	if (inventory)
	{
	    std::cout << "Inventory received:" << std::endl;

	    for (int i = 0; i < inventory->items_size(); i++) {
	    const rockin_msgs::Inventory_Item &item = inventory->items(i);
	      std::cout << "  Object " << i << ": " << item.object().description() << std::endl;
	      if (item.has_location()) std::cout << "    In location: " << item.location().description() << std::endl;
	      if (item.has_container()) std::cout << "    In container: " << item.container().description() << std::endl;
	      if (item.has_quantity()) std::cout << "    Quantity: " << item.quantity() << std::endl;
	    }
	}
}

int main(int argc, char **argv)
{
  llsfrb::YamlConfiguration config(CONFDIR);
  config.load("config.yaml");

  protobuf_comm::MessageRegister &message_register = client.message_register();
  message_register.add_message_type<rockin_msgs::BenchmarkState>();
  message_register.add_message_type<rockin_msgs::Inventory>();


  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.rockin.inventory-controller");
  builder = Gtk::Builder::create_from_file(std::string(SRCDIR) + std::string("/rockin_inventory_controller.glade"));

  Gtk::Window *window = 0;
  builder->get_widget("window1", window);
  window->set_title("RoCKIn Inventory-Controller");
  window->show_all();

  Gtk::Button *button_add_item_to_inventory = 0;
  Gtk::Button *button_remove_item_from_inventory = 0;
  Gtk::Button *button_preview_inventory = 0;
  Gtk::Button *button_update_inventory = 0;
  Gtk::Button *button_discard_changes_to_inventory = 0;
  Gtk::ComboBoxText *comboBoxText_choose_item_arena_inventory = 0;
  Gtk::ComboBoxText *comboBoxText_choose_item_complete_inventory = 0;

  builder->get_widget("button_add_item_to_inventory", button_add_item_to_inventory);
  builder->get_widget("button_remove_item_from_inventory", button_remove_item_from_inventory);
  builder->get_widget("button_preview_inventory", button_preview_inventory);
  builder->get_widget("button_update_inventory", button_update_inventory);
  builder->get_widget("button_discard_changes_to_inventory", button_discard_changes_to_inventory);
  builder->get_widget("comboBoxText_choose_item_arena_inventory", comboBoxText_choose_item_arena_inventory);
  builder->get_widget("comboBoxText_choose_item_complete_inventory", comboBoxText_choose_item_complete_inventory);

  Glib::signal_idle().connect(sigc::ptr_fun(&idle_handler));
  button_add_item_to_inventory->signal_clicked().connect(sigc::ptr_fun(&on_add_item_to_inventory_click));
  button_remove_item_from_inventory->signal_clicked().connect(sigc::ptr_fun(&on_remove_item_from_inventory_click));
  button_preview_inventory->signal_clicked().connect(sigc::ptr_fun(&on_preview_inventory_click));
  button_update_inventory->signal_clicked().connect(sigc::ptr_fun(&on_update_inventory_click));
  button_discard_changes_to_inventory->signal_clicked().connect(sigc::ptr_fun(&on_discard_changes_to_inventory_click));
  comboBoxText_choose_item_arena_inventory->signal_changed().connect(sigc::ptr_fun(&on_choose_item_arena_inventory_changed));
  comboBoxText_choose_item_complete_inventory->signal_changed().connect(sigc::ptr_fun(&on_choose_item_complete_inventory_changed));

  initialize_complete_inventory_comboBox();

  client.signal_received().connect(handle_message);
  client.signal_disconnected().connect(handle_disconnect);
  host = config.get_string("/llsfrb/shell/refbox-host");
  port = config.get_uint("/llsfrb/shell/refbox-port");
  client.async_connect(host.c_str(), port);

  return app->run(*window);
}
