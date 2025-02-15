<?php

namespace App\Controller;

use App\Entity\Employee;
use App\Form\CreateEmployeeType;
use App\Form\RemoveType;
use App\Operation\EmployeeOperation;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class EmployeeController extends AbstractController
{
    private EmployeeOperation $employeeOperation;

    public function __construct(EmployeeOperation $employeeOperation)
    {
        $this->employeeOperation = $employeeOperation;
    }

    #[Route('/employees', name: 'employees')]
    public function index(Request $request): Response
    {
        $criteria = [
            'firstName' => $request->query->get('firstName'),
            'lastName' => $request->query->get('lastName'),
            'email' => $request->query->get('email'),
            'phone' => $request->query->get('phone'),
            'website' => $request->query->get('website'),
            'description' => $request->query->get('description'),
            'roleTitles' => $request->query->all('roleTitles'),
        ];

        $fieldOfSort = $request->query->get('sort', 'id');
        $directionOfSort = strtolower($request->query->get('direction', 'asc')) === 'desc' ? 'desc' : 'asc';

        $page = max((int) $request->query->get('page', 1), 1);
        $limit = 4;

        $result = $this->employeeOperation->getPaginatedEmployees(
            array_filter($criteria, fn($value) => !empty($value)),
            $fieldOfSort,
            $directionOfSort,
            $page,
            $limit
        );

        return $this->render('employee/employee.html.twig', [
            'employees' => $result['employees'],
            'currentPage' => $page,
            'totalPages' => $result['totalPages'],
            'allRoles' => $result['allRoles'],
            'fieldOfSort' => $fieldOfSort,
            'directionOfSort' => $directionOfSort,
        ]);
    }

    #[Route('/employees/create', name: 'create_employee')]
    public function create(Request $request): Response
    {
        $newEmployee = new Employee();
        $form = $this->createForm(CreateEmployeeType::class, $newEmployee);
        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $this->employeeOperation->store($newEmployee);
            return $this->redirectToRoute('employees');
        }

        return $this->render('employee/create_employee.html.twig', [
            'form' => $form->createView(),
        ]);
    }

    #[Route('/employees/detail/{id}', name: 'employee_detail', requirements: ['id' => '\d+'])]
    public function show(int $id): Response
    {
        $employee = $this->employeeOperation->findById($id);

        if (!$employee) {
            return $this->redirectToRoute('error', ['message' => 'Zaměstnanec nebyl nalezen.']);
        }

        return $this->render('employee/detail.html.twig', [
            'employee' => $employee,
            'accounts' => $employee->getAccounts(),
        ]);
    }

    #[Route('/employees/edit/{id}', name: 'edit_employee', requirements: ['id' => '\d+'])]
    public function edit(int $id, Request $request): Response
    {
        $employee = $this->employeeOperation->findById($id);

        if (!$employee) {
            return $this->redirectToRoute('error', ['message' => 'Zaměstnanec nebyl nalezen.']);
        }

        $form = $this->createForm(CreateEmployeeType::class, $employee, ['is_edit' => true]);
        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $this->employeeOperation->store($employee);
            return $this->redirectToRoute('employee_detail', ['id' => $id]);
        }

        return $this->render('employee/edit_employee.html.twig', [
            'form' => $form->createView(),
            'employee' => $employee,
        ]);
    }

    #[Route('/employees/remove/{id}', name: 'remove_employee', requirements: ['id' => '\d+'], methods: ['GET', 'POST'])]
    public function remove(int $id, Request $request): Response
    {
        $employee = $this->employeeOperation->findById($id);

        if (!$employee) {
            $this->addFlash('error', 'Zaměstnanec nebyl nalezen.');
            return $this->redirectToRoute('employees');
        }

        if ($request->isMethod('POST')) {
            $this->employeeOperation->delete($employee);
            $this->addFlash('success', 'Zaměstnanec byl úspěšně odstraněn.');
            return $this->redirectToRoute('employees');
        }

        return $this->render('employee/remove_employee.html.twig', [
            'employee' => $employee,
        ]);
    }

    #[Route('/search', name: 'employee_search')]
    public function search(Request $request): Response
    {
        $criteria = [
            'firstName' => $request->query->get('firstName'),
            'lastName' => $request->query->get('lastName'),
            'email' => $request->query->get('email'),
            'phone' => $request->query->get('phone'),
            'website' => $request->query->get('website'),
            'description' => $request->query->get('description'),
            'roleTitles' => $request->query->all('roleTitles'),
        ];

        $employees = $this->employeeOperation->search(
            array_filter($criteria, fn($value) => !empty($value))
        );

        return $this->render('search/search.html.twig', [
            'employees' => $employees,
            'criteria' => $criteria,
            'allRoles' => $this->employeeOperation->findAllRoles(),
        ]);
    }
}
