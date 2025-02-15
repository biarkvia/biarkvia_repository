<?php

namespace App\Command;

use App\Entity\Employee;
use App\Entity\Account;
use App\Entity\Role;
use App\Repository\RoleRepository;
use Doctrine\ORM\EntityManagerInterface;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Attribute\AsCommand;

#[AsCommand(
    name: 'app:import-employees',
    description: 'Importovat zaměstnance a jejich účty do databáze.'
)]
class ImportEmployeesCommand extends Command
{
    private EntityManagerInterface $controlMyEntities;
    private RoleRepository $roleRepository;

    public function __construct(EntityManagerInterface $controlMyEntities, RoleRepository $roleRepository)
    {
        $this->controlMyEntities = $controlMyEntities;
        $this->roleRepository = $roleRepository;
        parent::__construct();
    }

    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        $employeesData = [
            [
                'firstName' => 'Vít',
                'lastName' => 'Slavík',
                'roles' => [
                    [
                        'title' => 'Vedoucí oddělení',
                        'description' => 'Vede oddělení, odpovídá za strategické plánování a kontrolu plnění úkolů.',
                        'isVisible' => true
                    ]
                ],
                'email' => 'vit.slavik@seznam.cz',
                'phone' => '+420784589667',
                'website' => 'http://www.vitslavik.com',
                'description' => 'Vedoucí oddělení s bohatými zkušenostmi v řízení projektů a vedení týmu. Specializuje se na optimalizaci procesů a zlepšování efektivity práce.',
                'accounts' => [
                    ['name' => 'Účet 1', 'type' => 'Username', 'expiration' => 'Permanentní']
                ]
            ],
            [
                'firstName' => 'Bohuslav',
                'lastName' => 'Havelka',
                'roles' => [
                    [
                        'title' => 'Programátor',
                        'description' => 'Vyvíjí software, vytváří kód a implementuje technická řešení pro potřeby společnosti.',
                        'isVisible' => true
                    ],
                    [
                        'title' => 'HR',
                        'description' => 'Zajišťuje nábor zaměstnanců, správu lidských zdrojů a organizaci aktivit zaměřených na rozvoj pracovníků.',
                        'isVisible' => true
                    ]
                ],
                'email' => 'bohuslav.havelka@seznam.cz',
                'phone' => '+420735221255',
                'website' => 'http://www.bohuslavhavelka.com',
                'description' => 'Programátor a HR specialista, který propojuje technické dovednosti s personalistikou. Odpovídá za vývoj softwaru a podporu zaměstnanců.',
                'accounts' => [
                    ['name' => 'Účet 2', 'type' => 'Card', 'expiration' => '03.10.2026']
                ]
            ],
            [
                'firstName' => 'Radmila',
                'lastName' => 'Králová',
                'roles' => [
                    [
                        'title' => 'Programátor',
                        'description' => 'Vyvíjí software, vytváří kód a implementuje technická řešení pro potřeby společnosti.',
                        'isVisible' => true
                    ],
                    [
                        'title' => 'Vedoucí týmu',
                        'description' => 'Řídí tým, koordinuje úkoly a rozděluje odpovědnosti mezi členy týmu k dosažení projektových cílů.',
                        'isVisible' => true
                    ]
                ],
                'email' => 'radmila.kralova@gmail.com',
                'phone' => '+420710533669',
                'website' => 'http://www.radmilakralova.cz',
                'description' => 'Programátorka a vedoucí týmu, zaměřená na řízení projektů v IT. Má výborné organizační schopnosti a zkušenosti s vedením technických týmů.',
                'accounts' => [
                    ['name' => 'Účet 3', 'type' => 'Password', 'expiration' => '25.12.2055']
                ]
            ],
            [
                'firstName' => 'Štěpánka',
                'lastName' => 'Zelená',
                'roles' => [
                    [
                        'title' => 'Programátor',
                        'description' => 'Vyvíjí software, vytváří kód a implementuje technická řešení pro potřeby společnosti.',
                        'isVisible' => true
                    ],
                    [
                        'title' => 'Web designér',
                        'description' => 'Navrhuje a vytváří vizuálně atraktivní a funkční rozhraní pro webové aplikace a stránky.',
                        'isVisible' => true
                    ]
                ],
                'email' => 'stepanka.zelena@gmail.com',
                'phone' => '+420752554361',
                'website' => 'http://www.stepankazelena.cz',
                'description' => 'Kreativní programátorka a web designérka, specializující se na uživatelské rozhraní a tvorbu vizuálně atraktivních webových stránek.',
                'accounts' => [
                    ['name' => 'Účet 4', 'type' => 'Card', 'expiration' => 'Permanentní']
                ]
            ],
            [
                'firstName' => 'Miloslav',
                'lastName' => 'Dvořák',
                'roles' => [
                    [
                        'title' => 'Systémový administrátor',
                        'description' => 'Zodpovídá za správu a podporu IT infrastruktury společnosti, zajišťuje bezproblémový provoz serverů a systémů.',
                        'isVisible' => true
                    ]
                ],
                'email' => 'miloslav.dvorak@gmail.com',
                'phone' => '+420745003443',
                'website' => 'http://www.miloslavdvorak.cz',
                'description' => 'Systémový administrátor s dlouholetou praxí v oblasti správy serverů a IT infrastruktury. Zajišťuje bezproblémový chod systémů ve firmě.',
                'accounts' => [
                    ['name' => 'Účet 5', 'type' => 'Password', 'expiration' => '01.01.2025'],
                    ['name' => 'Účet 6', 'type' => 'Username', 'expiration' => '01.01.2025']
                ]
            ],
            [
                'firstName' => 'Jan',
                'lastName' => 'Novák',
                'roles' => [
                    [
                        'title' => 'Analytik dat',
                        'description' => 'Zabývá se sběrem, analýzou a interpretací dat pro podporu obchodních rozhodnutí.',
                        'isVisible' => true
                    ]
                ],
                'email' => 'jan.novak@gmail.com',
                'phone' => '+420702334485',
                'website' => 'http://www.jannovak.com',
                'description' => 'Expert na analýzu dat se zaměřením na optimalizaci a zefektivnění obchodních procesů.',
                'accounts' => [
                    ['name' => 'Účet 7', 'type' => 'Username', 'expiration' => '12.12.2024']
                ]
            ],
            [
                'firstName' => 'Světlana',
                'lastName' => 'Konečná',
                'roles' => [
                    [
                        'title' => 'Marketingový specialista',
                        'description' => 'Vytváří a realizuje marketingové strategie na přilákání a udržení zákazníků.',
                        'isVisible' => true
                    ]
                ],
                'email' => 'svetlana.konecna@gmail.com',
                'phone' => '+420771234567',
                'website' => 'http://www.svetlanakonecna.com',
                'description' => 'Marketingová specialistka se zkušenostmi v digitálním marketingu a tvorbě obsahu pro sociální média.',
                'accounts' => [
                    ['name' => 'Účet 8', 'type' => 'Username', 'expiration' => 'Permanentní']
                ]
            ],
            [
                'firstName' => 'Tomáš',
                'lastName' => 'Beran',
                'roles' => [
                    [
                        'title' => 'Projektový manažer',
                        'description' => 'Řídí projekty, plánuje a koordinuje jejich realizaci, aby byly splněny cíle včas.',
                        'isVisible' => true
                    ]
                ],
                'email' => 'tomas.beran@gmail.com',
                'phone' => '+420729456789',
                'website' => 'http://www.tomasberan.com',
                'description' => 'Zkušený projektový manažer, který řídí projekty v oblasti IT a zajišťuje jejich úspěšnou realizaci.',
                'accounts' => [
                    ['name' => 'Účet 9', 'type' => 'Password', 'expiration' => '10.06.2026']
                ]
            ],
            [
                'firstName' => 'Petra',
                'lastName' => 'Růžičková',
                'roles' => [
                    [
                        'title' => 'HR',
                        'description' => 'Zajišťuje nábor zaměstnanců, správu lidských zdrojů a organizaci aktivit zaměřených na rozvoj pracovníků.',
                        'isVisible' => true
                    ]
                ],
                'email' => 'petra.ruzickova@gmail.com',
                'phone' => '+420710345678',
                'website' => 'http://www.petraruzickova.com',
                'description' => 'Odborník na lidské zdroje, který se specializuje na nábor a rozvoj zaměstnanců ve firmě.',
                'accounts' => [
                    ['name' => 'Účet 10', 'type' => 'Password', 'expiration' => 'Permanentní']
                ]
            ],
            [
                'firstName' => 'Martin',
                'lastName' => 'Kovář',
                'roles' => [
                    [
                        'title' => 'Vývojář softwaru',
                        'description' => 'Vyvíjí a udržuje softwarové aplikace, optimalizuje jejich výkon a funkčnost.',
                        'isVisible' => true
                    ]
                ],
                'email' => 'martin.kovar@gmail.com',
                'phone' => '+420701234167',
                'website' => 'http://www.martinkovar.com',
                'description' => 'Vývojář softwaru se zaměřením na backend technologie a optimalizaci databází.',
                'accounts' => [
                    ['name' => 'Účet 11', 'type' => 'Password', 'expiration' => '15.08.2025']
                ]
            ]
        ];

        $roleCache = [];

        foreach ($employeesData as $employeeData) {
            $employee = new Employee();
            $employee->setFirstName($employeeData['firstName']);
            $employee->setLastName($employeeData['lastName']);
            $employee->setEmail($employeeData['email']);
            $employee->setPhone($employeeData['phone']);
            $employee->setWebsite($employeeData['website']);
            $employee->setDescription($employeeData['description']);

            foreach ($employeeData['roles'] as $roleData) {
                if (isset($roleCache[$roleData['title']])) {
                    $role = $roleCache[$roleData['title']];
                } else {
                    $role = $this->roleRepository->findOneBy(['title' => $roleData['title']]);

                    if (!$role) {
                        $role = new Role();
                        $role->setTitle($roleData['title']);
                        $role->setDescription($roleData['description'] ?? '');
                        $role->setIsVisible($roleData['isVisible'] ?? true);

                        $this->controlMyEntities->persist($role);
                    }

                    $roleCache[$roleData['title']] = $role;
                }

                $employee->addRole($role);
            }

            foreach ($employeeData['accounts'] as $accountData) {
                $account = new Account();
                $account->setName($accountData['name']);
                $account->setType($accountData['type']);
                $account->setExpiration(
                    $accountData['expiration'] === 'Permanentní' ? null : new \DateTime($accountData['expiration'])
                );

                $employee->addAccount($account);
            }

            $this->controlMyEntities->persist($employee);
        }

        $this->controlMyEntities->flush();
        $output->writeln('Import zaměstnanců a jejich účtů byl úspěšně dokončen.');
        return Command::SUCCESS;
    }
}