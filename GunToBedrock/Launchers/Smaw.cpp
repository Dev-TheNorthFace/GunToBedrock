#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class SMAW : public Item {
public:
    SMAW() : Item("smaw", ItemType::CUSTOM) {
        maxAmmo = 1;
        currentAmmo = maxAmmo;
        reloadTime = 5.0f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        
        if (currentAmmo <= 0) {
            player.sendMessage("Votre SMAW est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < reloadTime) {
            player.sendMessage("Le lance-roquette recharge...");
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre SMAW est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::EXPLOSIVE)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::EXPLOSIVE, 1);
            player.sendMessage("Votre SMAW a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas de roquettes pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float reloadTime;
    float lastFireTime;

    void shoot(Player &player) {
        Projectile &rocket = player.spawnProjectile(ProjectileType::FIREBALL);
        rocket.setDamage(50);
        rocket.setVelocity(player.getDirection() * 10.0f);
        player.playSound(SoundType::EXPLOSION);
        player.sendMessage("BOUM. La roquette a explosé.");
    }
};

extern "C" void init() {
    Item::registerItem<SMAW>("smaw");
}