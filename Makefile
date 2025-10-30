# === Variables ===
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -O2
TARGET := main

# === Répertoires ===
SRC_DIR := .
OBJ_DIR := build
HELP_DIR := help
INSTALL_DIR := install
PURGE_DIR := purge
UPDATE_DIR := update
UPGRADE_DIR := upgrade

# === Fichiers sources ===
SRC := $(SRC_DIR)/main.cpp \
       $(HELP_DIR)/help.cpp \
       $(INSTALL_DIR)/install.cpp \
       $(PURGE_DIR)/purge.cpp \
       $(UPDATE_DIR)/update.cpp \
       $(UPGRADE_DIR)/upgrade.cpp

# === Fichiers objets ===
OBJ := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

# === Règles ===

# Règle par défaut
all: $(TARGET)

# Lien final
$(TARGET): $(OBJ)
	@echo "🔗 Édition des liens..."
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "✅ Compilation terminée : ./$(TARGET)"

# Compilation des fichiers objets
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "⚙️  Compilation de $< ..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Création des sous-dossiers dans build/
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/$(HELP_DIR) \
	           $(OBJ_DIR)/$(INSTALL_DIR) \
	           $(OBJ_DIR)/$(PURGE_DIR) \
	           $(OBJ_DIR)/$(UPDATE_DIR) \
	           $(OBJ_DIR)/$(UPGRADE_DIR)

# Nettoyage
clean:
	@echo "🧹 Nettoyage..."
	rm -rf $(OBJ_DIR) $(TARGET)

# Rebuild complet
rebuild: clean all

.PHONY: all clean rebuild

