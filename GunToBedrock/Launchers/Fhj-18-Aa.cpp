#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class FHJ18AA : public Item {
public:
    FHJ18AA() : Item("fhj18_aa", ItemType::CUSTOM) {
        maxAmmo = 1;
        currentAmmo = maxAmmo;
        reloadTime = 8.0f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        
        if (currentAmmo <= 0) {
            player.sendMessage("Votre FHJ-18 AA est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < reloadTime) {
            player.sendMessage("Le missile recharge...");
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre FHJ-18 AA est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::EXPLOSIVE)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::EXPLOSIVE, 1);
            player.sendMessage("Votre FHJ-18 AA a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas de missiles pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float reloadTime;
    float lastFireTime;

    void shoot(Player &player) {
        Projectile &missile = player.spawnProjectile(ProjectileType::FIREBALL);
        missile.setDamage(75);
        missile.setVelocity(player.getDirection() * 10.0f);
        missile.setGuided(true);
        Entity* target = findAerialTarget(player);
        if (target) {
            missile.setTarget(target);
            player.sendMessage("Missile lancé. Suivi de la cible aérienne.");
        } else {
            player.sendMessage("Aucune cible aérienne détectée. Le missile se dirige au hasard.");
        }

        player.playSound(SoundType::LAUNCH);
    }

    Entity* findAerialTarget(Player &player) {
        for (Entity &entity : Minecraft::getEntities()) {
            if (entity.isFlying()) {
                return &entity;
            }
        }
        return nullptr;
    }
};

extern "C" void init() {
    Item::registerItem<FHJ18AA>("fhj18_aa");
}